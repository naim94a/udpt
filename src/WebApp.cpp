/*
 *	Copyright © 2012-2017 Naim A.
 *
 *	This file is part of UDPT.
 *
 *		UDPT is free software: you can redistribute it and/or modify
 *		it under the terms of the GNU General Public License as published by
 *		the Free Software Foundation, either version 3 of the License, or
 *		(at your option) any later version.
 *
 *		UDPT is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *		GNU General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with UDPT.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/thread.h>
#include "WebApp.hpp"
#include "logging.hpp"


namespace UDPT {
    WebApp::WebApp(UDPT::Data::DatabaseDriver &db): m_db(db) {
        //TODO: check if platform is windows, add to CMakeLists

        ::evthread_use_pthreads();

        m_eventBase = std::shared_ptr<struct event_base>(::event_base_new(), ::event_base_free);
        if (nullptr == m_eventBase) {
            LOG_ERR("webapp", "Failed to create event base");
            throw std::exception();
        }

        m_httpServer = std::shared_ptr<struct evhttp>(::evhttp_new(m_eventBase.get()), ::evhttp_free);
        if (nullptr == m_httpServer) {
            LOG_ERR("webapp", "Failed to create http base");
            throw std::exception();
        }

        if (0 != ::evhttp_bind_socket(m_httpServer.get(), "127.0.0.1", 8080)) {
            LOG_ERR("webapp", "Failed to bind socket");
            throw std::exception();
        }

        ::evhttp_set_allowed_methods(m_httpServer.get(), EVHTTP_REQ_GET | EVHTTP_REQ_POST);

        ::evhttp_set_gencb(m_httpServer.get(), viewNotFound, this);
        ::evhttp_set_cb(m_httpServer.get(), "/", [](struct evhttp_request *req, void *){
            setCommonHeaders(req);
            sendReply(req, 200, "OK", HOME_PAGE);
        }, this);
        ::evhttp_set_cb(m_httpServer.get(), "/announce", [](struct evhttp_request *req, void *){
            setCommonHeaders(req);
            sendReply(req, 200, "OK", ANNOUNCE_PAGE);
        }, this);
        ::evhttp_set_cb(m_httpServer.get(), "/api/torrents", viewApiTorrents, this);
    }

    WebApp::~WebApp() {
        try {
            if (m_workerThread.joinable()) {
                m_workerThread.join();
            }
        } catch (...) {
            LOG_FATAL("webapp", "exception thrown @ WebApp termination.");
        }
    }

    void WebApp::start() {
        LOG_INFO("webapp", "Starting WebApp");
        LOG_INFO("webapp", "compiled with libevent " << LIBEVENT_VERSION << ", running with " << event_get_version());
        m_workerThread = std::thread(workerThread, this);
    }

    void WebApp::stop() {
        if (!m_isRunning) {
            return;
        }
        m_isRunning = false;

        LOG_INFO("webapp", "Requesting WebApp to stop");
        ::event_base_loopbreak(m_eventBase.get());
    }

    const std::string WebApp::ANNOUNCE_PAGE = "d14:failure reason41:udpt: This is a udp tracker, not HTTP(s).e";
    const std::string WebApp::NOT_FOUND_PAGE = "<h2>Not Found</h2>";
    const std::string WebApp::HOME_PAGE = "<html>"
            "<head>"
            "<title>UDPT</title>"
            "</head>"
            "<body>"
            "<h2>UDPT Tracker</h2>"
            "<div style=\"text-align: center; font-size: small;\"><a href=\"https://github.com/naim94a/udpt\">https://github.com/naim94a/udpt</a></div>"
            "</body>"
            "</html>";

    void WebApp::workerThread(WebApp *app) {
        app->m_isRunning = true;
        ::event_base_dispatch(app->m_eventBase.get());

        LOG_INFO("webapp", "Worker " << std::this_thread::get_id() << " exited");
    }

    void WebApp::viewApiTorrents(struct ::evhttp_request *req, void *app) {
        setCommonHeaders(req);
    }

    void WebApp::viewNotFound(struct ::evhttp_request *req, void *app) {
        setCommonHeaders(req);
        sendReply(req, HTTP_NOTFOUND, "Not Found", NOT_FOUND_PAGE);
    }

    void WebApp::setCommonHeaders(struct ::evhttp_request *req) {
        struct evkeyvalq *resp_headers = ::evhttp_request_get_output_headers(req);
        ::evhttp_add_header(resp_headers, "Server", "udpt");
    }

    void WebApp::sendReply(struct ::evhttp_request *req, int code, const char *reason, const std::string &response) {
        sendReply(req, code, reason, response.c_str(), response.length());
    }

    void WebApp::sendReply(struct ::evhttp_request *req, int code, const char *reason, const char *response, size_t len) {
        std::shared_ptr<struct evbuffer> resp (::evbuffer_new(), ::evbuffer_free);

        if (nullptr == resp) {
            LOG_ERR("webapp", "evbuffer_new() failed to allocate buffer");
            goto error;
        }

        {
            int result = ::evbuffer_add_reference(resp.get(), response, len, nullptr, nullptr);
            if (0 != result) {
                LOG_ERR("webapp", "evbuffer_add_reference() returned " << result);
                goto error;
            }
        }

        ::evhttp_send_reply(req, code, reason, resp.get());

        // This is C++, and this is the C approach, maybe fix this in the future?
        error:
        ::evhttp_send_error(req, HTTP_INTERNAL, "Internal Server Error");
    }
}
