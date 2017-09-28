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

        ::evhttp_set_cb(m_httpServer.get(), "/", viewHomepage, this);
        ::evhttp_set_cb(m_httpServer.get(), "/announce", viewAnnounce, this);
        ::evhttp_set_cb(m_httpServer.get(), "/api/torrents", viewApiTorrents, this);

        m_homeTemplate = std::shared_ptr<struct evbuffer>(::evbuffer_new(), ::evbuffer_free);

        ::evbuffer_add_printf(m_homeTemplate.get(), "<html>"
                "<head>"
                "<title>UDPT</title>"
                "</head>"
                "<body>"
                "<h2>UDPT Tracker</h2>"
                "<div style=\"text-align: center; font-size: small;\"><a href=\"https://github.com/naim94a/udpt\">https://github.com/naim94a/udpt</a></div>"
                "</body>"
                "</html>");
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

    void WebApp::workerThread(WebApp *app) {
        app->m_isRunning = true;
        ::event_base_dispatch(app->m_eventBase.get());

        LOG_INFO("webapp", "Worker " << std::this_thread::get_id() << " exited");
    }

    void WebApp::viewHomepage(struct ::evhttp_request *req, void *ctx) {
        WebApp *app = reinterpret_cast<WebApp*>(ctx);

        struct evkeyvalq *resp_headers = ::evhttp_request_get_output_headers(req);
        ::evhttp_add_header(resp_headers, "Server", "udpt");

        struct evbuffer *resp = evbuffer_new();
        ::evbuffer_add_buffer_reference(resp, app->m_homeTemplate.get());

        ::evhttp_send_reply(req, 200, "OK", resp);

        evbuffer_free(resp);
    }

    void WebApp::viewAnnounce(struct ::evhttp_request *req, void *app) {

    }

    void WebApp::viewApiTorrents(struct ::evhttp_request *req, void *app) {

    }
}
