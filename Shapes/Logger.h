#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>

class Logger {
public:
    /**
     * Logger aquisition methods.
     */
    static Logger& getErrorLogger()
    {
        if (_errorLogger == 0) {
            _errorLogger = new Logger();
        }
        return *_errorLogger;
    }

    static Logger& getWarnLogger()
    {
        if (_warnLogger == 0) {
            _warnLogger = new Logger();
        }
        return *_warnLogger;
    }

    static Logger& getInfoLogger()
    {
        if (_infoLogger == 0) {
            _infoLogger = new Logger();
        }
        return *_infoLogger;
    }


    /**
     * Logger setup.
     */
    void setEnabled(bool bValue)
    {
        _isEnabled = bValue;
    }


    /**
     * Log methods.
     */
    Logger& operator<< (int i)
    {
        if (_isEnabled) {
            std::cout << i;
        }
        return *this;
    }

    Logger& operator<< (unsigned int ui)
    {
        if (_isEnabled) {
            std::cout << ui;
        }
        return *this;
    }

    Logger& operator<< (long l)
    {
        if (_isEnabled) {
            std::cout << l;
        }
        return *this;
    }

    Logger& operator<< (unsigned long ul)
    {
        if (_isEnabled) {
            std::cout << ul;
        }
        return *this;
    }

    Logger& operator<< (double d)
    {
        if (_isEnabled) {
            std::cout << d;
        }
        return *this;
    }

    Logger& operator<< (const char* s)
    {
        if (_isEnabled) {
            std::cout << s;
        }
        return *this;
    }


    /**
     * Quick log methods.
     */
    static void logError(const char* s)
    {
        getErrorLogger() << s;
    }

    static void logWarn(const char* s)
    {
        getWarnLogger() << s;
    }

    static void logInfo(const char* s)
    {
        getInfoLogger() << s;
    }

    static void logError(int i)
    {
        getErrorLogger() << i;
    }

    static void logWarn(int i)
    {
        getWarnLogger() << i;
    }

    static void logInfo(int i)
    {
        getInfoLogger() << i;
    }

    static void logError(unsigned int ui)
    {
        getErrorLogger() << ui;
    }

    static void logWarn(unsigned int ui)
    {
        getWarnLogger() << ui;
    }

    static void logInfo(unsigned int ui)
    {
        getInfoLogger() << ui;
    }

    static void logError(long l)
    {
        getErrorLogger() << l;
    }

    static void logWarn(long l)
    {
        getWarnLogger() << l;
    }

    static void logInfo(long l)
    {
        getInfoLogger() << l;
    }

    static void logError(unsigned long ul)
    {
        getErrorLogger() << ul;
    }

    static void logWarn(unsigned long ul)
    {
        getWarnLogger() << ul;
    }

    static void logInfo(unsigned long ul)
    {
        getInfoLogger() << ul;
    }

    static void logError(double d)
    {
        getErrorLogger() << d;
    }

    static void logWarn(double d)
    {
        getWarnLogger() << d;
    }

    static void logInfo(double d)
    {
        getInfoLogger() << d;
    }

private:
    Logger() :
        _isEnabled(false)
    {
    }

    bool _isEnabled;

    static Logger* _errorLogger;
    static Logger* _warnLogger;
    static Logger* _infoLogger;
};

#endif /* LOGGER_H_ */
