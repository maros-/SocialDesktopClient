/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#ifndef CORE_LOG_H_
#define CORE_LOG_H_

#include <iostream>
#include <sstream>
#include <string>
#include <map>

/**
 * Macro defined for logging purposes.
 *
 * If assembly is compiled with LOG_PRODUCER=NAME directive
 * all logs within the assembly include NAME prefix.
 */
#ifdef LOG_PRODUCER

#define TOKEN_TO_STRING(TOK) # TOK
#define STRINGIZE_TOKEN(TOK) TOKEN_TO_STRING(TOK)
#define LOG(level) \
  if(sdc::Log::level > sdc::Log::GetGlobalLevel()) ; \
  else sdc::Log().Get(sdc::Log::level, STRINGIZE_TOKEN(LOG_PRODUCER))

#else

#define LOG(level) \
  if(sdc::Log::level > sdc::Log::GetGlobalLevel()) ; \
  else sdc::Log().Get(sdc::Log::level, "Core")

#endif

namespace sdc {

/**
 * SocialDesktopClient logger.
 * It provides leveled logging capabilieties.
 */
class Log {
 public:
  enum Level {
    ERROR,
    WARNING,
    INFO,
    TRACE,
    DEBUG,
    DEBUG1,
    DEBUG2,
    DEBUG3,
    DEBUG4
  };
  Log();
  virtual ~Log();
  std::ostringstream& Get(Level level, std::string producer);

  static void SetGlobalLevel(Level level);
  static Level GetGlobalLevel();
  static void TimeFormat(const std::string &);

 protected:
  std::ostringstream os;
  Level current_level_;
  static Level global_level_;
  static std::string level_name_[];
  static std::string time_format_;

 private:
  Log(const Log&);
  Log& operator=(const Log&);

  std::string current_time();
};

} // namespace sdc

#endif // CORE_LOG_H_
