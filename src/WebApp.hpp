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

#pragma once
#include <event2/http.h>
#include <thread>
#include <condition_variable>

#include "db/database.hpp"

namespace UDPT
{
    class WebApp {
    public:
        WebApp(UDPT::Data::DatabaseDriver& db);

        virtual ~WebApp();

        void start();

        void stop();

    private:
        UDPT::Data::DatabaseDriver& m_db;

        std::thread m_workerThread;
        std::atomic_bool m_isRunning;

        // Be Aware: The order of these members are important
        // we wouldn't want to free event_base before http_server...
        std::shared_ptr<struct event_base> m_eventBase;
        std::shared_ptr<struct evhttp> m_httpServer;
        std::shared_ptr<struct evbuffer> m_homeTemplate;

        static void workerThread(WebApp *);


        static void viewHomepage(struct ::evhttp_request *, void *);

        static void viewAnnounce(struct ::evhttp_request *, void *);

        static void viewApiTorrents(struct ::evhttp_request *, void *);

        static void setCommonHeaders(struct ::evhttp_request *);
    };
}