#pragma once

#include "common/smart_ptr.h"
#include "common/net/net.h"

#include "core/connection_types.h"
#include "core/ssh_info.h"

namespace fastoredis
{
    class IConnectionSettings
    {
    public:
        virtual ~IConnectionSettings();

        std::string connectionName() const;
        void setConnectionName(const std::string& name);

        connectionTypes connectionType() const;

        bool loggingEnabled() const;
        void setLoggingEnabled(bool isLogging);

        virtual std::string toString() const = 0;
        virtual IConnectionSettings* clone() const = 0;

    protected:
        IConnectionSettings(const std::string& connectionName, connectionTypes type);

        std::string connectionName_;
        bool logging_enabled_;
        const connectionTypes type_;
    };

    class IConnectionSettingsBase
            : public IConnectionSettings
    {
    public:
        virtual ~IConnectionSettingsBase();
        std::string hash() const;

        std::string loggingPath() const;

        void setHost(const common::net::hostAndPort& host);
        void setConnectionNameAndUpdateHash(const std::string& name);

        virtual std::string commandLine() const = 0;
        virtual void setCommandLine(const std::string& line) = 0;

        virtual std::string host() const = 0;
        virtual void setHost(const std::string& host) = 0;
        virtual int port() const = 0;
        virtual void setPort(int port) = 0;

        std::string fullAddress() const;

        static IConnectionSettingsBase* createFromType(connectionTypes type, const std::string& conName = std::string());
        static IConnectionSettingsBase* fromString(const std::string& val);

        virtual std::string toString() const;

        uint32_t loggingMsTimeInterval() const;

        SSHInfo sshInfo() const;
        void setSshInfo(const SSHInfo& info);

        bool isRoot() const;

    protected:
        virtual std::string toCommandLine() const = 0;
        virtual void initFromCommandLine(const std::string& val) = 0;
        IConnectionSettingsBase(const std::string& connectionName, connectionTypes type);

    private:
        using IConnectionSettings::setConnectionName;

        std::string hash_;        
        SSHInfo sshInfo_;
        bool isRoot_;
    };

    const char *useHelpText(connectionTypes type);
    std::string defaultCommandLine(connectionTypes type);

    typedef common::shared_ptr<IConnectionSettingsBase> IConnectionSettingsBaseSPtr;

    class IClusterSettingsBase
            : public IConnectionSettings
    {
    public:
        typedef std::vector<IConnectionSettingsBaseSPtr> cluster_connection_type;
        cluster_connection_type nodes() const;
        IConnectionSettingsBaseSPtr rootSetting() const;

        void addNode(IConnectionSettingsBaseSPtr node);

        static IClusterSettingsBase* createFromType(connectionTypes type, const std::string& conName = std::string());
        static IClusterSettingsBase* fromString(const std::string& val);

        virtual std::string toString() const;

    protected:
        IClusterSettingsBase(const std::string& connectionName, connectionTypes type);

    private:
        cluster_connection_type clusters_nodes_;
    };

    typedef common::shared_ptr<IClusterSettingsBase> IClusterSettingsBaseSPtr;
}
