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
#include <string>
#include <ostream>
#include <thread>
#include "MessageQueue.hpp"

namespace UDPT {
    namespace Logging {

        enum Severity {
            UNSET = 0,
            DEBUG = 10,
            INFO = 20,
            WARNING = 30,
            ERROR = 40,
            FATAL = 50
        };

        struct LogEntry {
            std::chrono::time_point<std::chrono::system_clock> when;
            Severity severity;
            const std::string& channel;
            std::string message;
        };

        class Logger {
        public:
            static Logger& getLogger();

            void log(Severity severity, const std::string& channel, const std::string& message);

            void addStream(std::ostream *, Severity minSeverity=INFO);

            void flush();

        private:
            Logger();
            virtual ~Logger();

            std::vector<std::pair<std::ostream*, UDPT::Logging::Severity>> m_outputStreams;
            UDPT::Utils::MessageQueue<struct LogEntry> m_queue;
        };

    }
}
