#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <string>
#include <iostream>

namespace Blessed
{

class Logger
{
public:

     enum LogLevel
     {
          INFO = 1,
          WARNING,
          ERROR,
          DEBUG
     };

     Logger( std::mutex& logMutex, const std::string& path , const LogLevel &logLevel = LogLevel::INFO )
     : logMutex_( logMutex ), logPath_( path ), logLevel_( logLevel ), info( Info( *this ) ),
     error( Error( *this ) ), warning( Warning( *this ) ), debug( Debug( *this ) )
     {
          // info = Info( *this );
     }

     LogLevel getLogLevel()
     {
          return logLevel_;
     }

     std::string getLogPath()
     {
          return logPath_;
     }

     class Info
     {
     public:
          Info( Logger& parent ) : parent_( parent ) {}

          template<class T>
          Info& operator<<( const T& param )
          {
               if ( LogLevel::INFO <= parent_.logLevel_ )
               {
                    std::lock_guard<std::mutex> lock( parent_.logMutex_ );
                    std::cout << param;
               }
               return *this;
          }
     private:
          Logger& parent_;
     };

     class Warning
     {
     public:
          Warning( Logger& parent ) : parent_( parent ) {}

          template<class T>
          Warning& operator<<( const T& param )
          {
               if ( LogLevel::WARNING <= parent_.logLevel_ )
               {
                    std::lock_guard<std::mutex> lock( parent_.logMutex_ );
                    std::cout << param;
               }
               return *this;
          }
     private:
          Logger& parent_;
     };

     class Error
     {
     public:
          Error( Logger& parent ) : parent_( parent ) {}

          template<class T>
          Error& operator<<( const T& param )
          {
               if ( LogLevel::ERROR <= parent_.logLevel_ )
               {
                    std::lock_guard<std::mutex> lock( parent_.logMutex_ );
                    std::cout << param;
               }
               return *this;
          }
     private:
          Logger& parent_;
     };

     class Debug
     {
     public:
          Debug( Logger& parent ) : parent_( parent ) {}

          template<class T>
          Debug& operator<<( const T& param )
          {
               if ( LogLevel::DEBUG <= parent_.logLevel_ )
               {
                    std::lock_guard<std::mutex> lock( parent_.logMutex_ );
                    std::cout << param;
               }
               return *this;
          }
     private:
          Logger& parent_;
     };

private:
     std::mutex& logMutex_;
     std::string logPath_;
     LogLevel logLevel_;

public:
     Info info;
     Debug debug;
     Error error;
     Warning warning;
};

/* USAGE
     std::mutex logMutex;

     Blessed::Logger logger( logMutex, "", Blessed::Logger::LogLevel::DEBUG );
     logger.info << "qq" << 1 << 'c' << "\n";*/

}

#endif