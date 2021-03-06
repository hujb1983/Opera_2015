#include <UtilityIni.h>
#include "TemplateServerConfig.h"
#include "TemplateMainServer.h"
#include "TemplateSeasoning.h"

TemplateServerConfig::TemplateServerConfig()
{
    memset( this, 0x0, sizeof(TemplateServerConfig) );
}

TemplateServerConfig::~TemplateServerConfig()
{
}

void TemplateServerConfig::InitConfig()
{

    m_pLoginSession = NULL;
	m_pAgentSession = NULL;
	m_pLobbySession = NULL;
	m_pGameSession = NULL;
	m_pDBSession = NULL;

	memset( m_desc, 0x0, sizeof(m_desc) );

	m_desc[0].dwSyncHandlerKey			= SERVER_SYNCHANDLER;
	m_desc[0].dwMaxAcceptSession		= 5;
	m_desc[0].dwMaxConnectSession		= 5;
	m_desc[0].dwSendBufferSize			= 41943040;//(40M)
	m_desc[0].dwRecvBufferSize		 	= 41943040;//(40M)
	m_desc[0].dwTimeOut				    = 0;
	m_desc[0].dwNumberOfAcceptThreads	= 1;
	m_desc[0].dwNumberOfIoThreads		= 2;//(发送和接收)
	m_desc[0].dwNumberOfConnectThreads	= 1;// Agent Server don't Active connect.
	m_desc[0].dwMaxPacketSize			= 40960; //(每个包大小)

	m_desc[1].dwSyncHandlerKey 			= CLIENT_SYNCHANDLER;
	m_desc[1].dwMaxAcceptSession		= 999;
	m_desc[1].dwMaxConnectSession		= 0;
	m_desc[1].dwSendBufferSize			= 40960; //40960
	m_desc[1].dwRecvBufferSize			= 40960; //40960
	m_desc[1].dwTimeOut				    = 0;
	m_desc[1].dwNumberOfAcceptThreads	= 1;
	m_desc[1].dwNumberOfIoThreads		= 2;//(发送和接收)
	m_desc[1].dwNumberOfConnectThreads	= 0;
	m_desc[1].dwMaxPacketSize			= 4096;	//(每个包大小)

	m_wPort[0] = 8100;
    m_wPort[1] = 1234;
}

void TemplateServerConfig::LoadCompleteServerConfig()
{
    m_desc[0].dwSyncHandlerKey			|= SERVER_SYNCHANDLER;
	m_desc[0].dwMaxAcceptSession		|= m_ddesc[0].dwMaxAcceptSession;
	m_desc[0].dwMaxConnectSession		|= m_ddesc[0].dwMaxConnectSession;
	m_desc[0].dwSendBufferSize			|= m_ddesc[0].dwSendBufferSize;//(40M)
	m_desc[0].dwRecvBufferSize		 	|= m_ddesc[0].dwRecvBufferSize;//(40M)
	m_desc[0].dwTimeOut				    |= m_ddesc[0].dwTimeOut;
	m_desc[0].dwNumberOfAcceptThreads	|= m_ddesc[0].dwNumberOfAcceptThreads;
	m_desc[0].dwNumberOfIoThreads		|= m_ddesc[0].dwNumberOfIoThreads;//(发送和接收)
	m_desc[0].dwNumberOfConnectThreads	|= m_ddesc[0].dwNumberOfConnectThreads;// Agent Server don't Active connect.
	m_desc[0].dwMaxPacketSize			|= m_ddesc[0].dwMaxPacketSize; //(每个包大小)

	m_desc[1].dwSyncHandlerKey 			|= CLIENT_SYNCHANDLER;
	m_desc[1].dwMaxAcceptSession		|= m_ddesc[1].dwMaxAcceptSession;
	m_desc[1].dwMaxConnectSession		|= m_ddesc[1].dwMaxConnectSession;
	m_desc[1].dwSendBufferSize			|= m_ddesc[1].dwSendBufferSize; //40960
	m_desc[1].dwRecvBufferSize			|= m_ddesc[1].dwRecvBufferSize; //40960
	m_desc[1].dwTimeOut				    |= m_ddesc[1].dwTimeOut;
	m_desc[1].dwNumberOfAcceptThreads	|= m_ddesc[1].dwNumberOfAcceptThreads;
	m_desc[1].dwNumberOfIoThreads		|= m_ddesc[1].dwNumberOfIoThreads;//(发送和接收)
	m_desc[1].dwNumberOfConnectThreads	|= m_ddesc[1].dwNumberOfConnectThreads;
	m_desc[1].dwMaxPacketSize			|= m_ddesc[1].dwMaxPacketSize;//(每个包大小)
}

void TemplateServerConfig::SetServerType( eSERVER_TYPE byServerType ) {
	m_byServerType = byServerType;
}

void TemplateServerConfig::LoadServerConfig( const char * szConfig )
{
	UtilityIni _ini( szConfig );

	m_byIOSize = 0;
	if (m_byServerType==LOGIN_SERVER) {
		LoadLocalServer( "LOGIN_SERVER", _ini);
		LoadConnectObjects("LOGIN_SERVER", _ini);
	}
	else if (m_byServerType==AGENT_SERVER)
	{
		LoadLocalServer( "AGENT_SERVER", _ini);
		LoadConnectObjects("AGENT_SERVER", _ini);
	}
	else if (m_byServerType==LOBBY_SERVER)
	{
		LoadLocalServer( "LOBBY_SERVER", _ini);
		LoadConnectObjects("LOBBY_SERVER", _ini);
	}
	else if (m_byServerType==GAME_SERVER)
	{
		LoadLocalServer( "GAME_SERVER", _ini);
		LoadConnectObjects("GAME_SERVER", _ini);
	}
	else if (m_byServerType==DB_SERVER)
	{
		LoadLocalServer( "DB_SERVER", _ini);
		LoadConnectObjects("DB_SERVER", _ini);
	}
}

void TemplateServerConfig::InitDafaultConfig( const char * szConfig )
{
    UtilityIni _ini( szConfig );
    LoadDafaultConfig( _ini );
    LoadDatabaseConfig( _ini );
}

void TemplateServerConfig::LoadDafaultConfig( UtilityIni & _ini )
{
    m_byDafaultIOSize = 0;
    char szServerTypeName[128] = {0};

    strcat( szServerTypeName, "DAFAULT_SERVER" );
    m_ddesc[m_byDafaultIOSize].dwSyncHandlerKey = SERVER_SYNCHANDLER;
    m_ddesc[m_byDafaultIOSize].dwMaxAcceptSession = _ini.GetInteger(szServerTypeName, "MaxAcceptSession" );
    m_ddesc[m_byDafaultIOSize].dwMaxConnectSession = _ini.GetInteger(szServerTypeName, "MaxConnectSession" );
    m_ddesc[m_byDafaultIOSize].dwSendBufferSize = _ini.GetInteger(szServerTypeName, "SendBufferSize" );
    m_ddesc[m_byDafaultIOSize].dwRecvBufferSize = _ini.GetInteger(szServerTypeName, "RecvBufferSize" );
    m_ddesc[m_byDafaultIOSize].dwTimeOut = _ini.GetInteger(szServerTypeName, "TimeOut" );
    m_ddesc[m_byDafaultIOSize].dwNumberOfAcceptThreads = _ini.GetInteger(szServerTypeName, "NumberOfAcceptThreads" );
    m_ddesc[m_byDafaultIOSize].dwNumberOfIoThreads = _ini.GetInteger(szServerTypeName, "NumberOfIoThreads" );
    m_ddesc[m_byDafaultIOSize].dwNumberOfConnectThreads = _ini.GetInteger(szServerTypeName, "NumberOfConnectThreads" );
    m_ddesc[m_byDafaultIOSize].dwMaxPacketSize = _ini.GetInteger(szServerTypeName, "MaxPacketSize" );
    m_byDafaultIOSize++;

    szServerTypeName[0] = '\0';
    strcat( szServerTypeName, "DAFAULT_CLIENT" );
    m_ddesc[m_byDafaultIOSize].dwSyncHandlerKey = CLIENT_SYNCHANDLER;
    m_ddesc[m_byDafaultIOSize].dwMaxAcceptSession = _ini.GetInteger(szServerTypeName, "MaxAcceptSession" );
    m_ddesc[m_byDafaultIOSize].dwMaxConnectSession = _ini.GetInteger(szServerTypeName, "MaxConnectSession" );
    m_ddesc[m_byDafaultIOSize].dwSendBufferSize = _ini.GetInteger(szServerTypeName, "SendBufferSize" );
    m_ddesc[m_byDafaultIOSize].dwRecvBufferSize = _ini.GetInteger(szServerTypeName, "RecvBufferSize" );
    m_ddesc[m_byDafaultIOSize].dwTimeOut = _ini.GetInteger(szServerTypeName, "TimeOut" );
    m_ddesc[m_byDafaultIOSize].dwNumberOfAcceptThreads = _ini.GetInteger(szServerTypeName, "NumberOfAcceptThreads" );
    m_ddesc[m_byDafaultIOSize].dwNumberOfIoThreads = _ini.GetInteger(szServerTypeName, "NumberOfIoThreads" );
    m_ddesc[m_byDafaultIOSize].dwNumberOfConnectThreads = _ini.GetInteger(szServerTypeName, "NumberOfConnectThreads" );
    m_ddesc[m_byDafaultIOSize].dwMaxPacketSize = _ini.GetInteger(szServerTypeName, "MaxPacketSize" );
    m_byDafaultIOSize++;
}

void TemplateServerConfig::LoadDatabaseConfig( UtilityIni & _ini )
{
    std::string szIP = _ini.GetString("DATABASE", "DBAddress" );
    std::string szDB = _ini.GetString("DATABASE", "DBName" );
    std::string szUsername = _ini.GetString("DATABASE", "DBUsername" );
    std::string szPasswork = _ini.GetString("DATABASE", "DBPasswork" );

	memcpy( m_szDBAddress, szIP.c_str(), szIP.size() );
	memcpy( m_szDBName, szDB.c_str(), szDB.size() );
	memcpy( m_szDBUsername, szUsername.c_str(), szUsername.size() );
	memcpy( m_szDBPasswork, szPasswork.c_str(), szPasswork.size() );

	TemplateSeasoning system;
    memcpy( system.GetDBAddress(),  szIP.c_str(), szIP.size() );
    memcpy( system.GetDBName(), szDB.c_str(), szDB.size() );
	memcpy( system.GetDBUsername(), szUsername.c_str(), szUsername.size() );
	memcpy( system.GetDBPassword(), szPasswork.c_str(), szPasswork.size() );
}

void TemplateServerConfig::LoadLocalServer( const char * szServerTypeName, UtilityIni & _ini )
{
	std::string szHandler="";

	m_wPort[0] = 0;
	m_wPort[1] = 0;

	szHandler = _ini.GetString(szServerTypeName, "ServerHandler", "false" );
	if (strcmp(szHandler.c_str(),"true")==0 ) {
		m_desc[0].dwSyncHandlerKey = SERVER_SYNCHANDLER;
		m_desc[0].dwMaxAcceptSession = _ini.GetInteger(szServerTypeName, "ServerMaxAcceptSession" );
		m_desc[0].dwMaxConnectSession = _ini.GetInteger(szServerTypeName, "ServerMaxConnectSession" );
		m_desc[0].dwSendBufferSize = _ini.GetInteger(szServerTypeName, "ServerSendBufferSize" );
		m_desc[0].dwRecvBufferSize = _ini.GetInteger(szServerTypeName, "ServerRecvBufferSize" );
		m_desc[0].dwTimeOut = _ini.GetInteger(szServerTypeName, "ServerTimeOut" );
		m_desc[0].dwNumberOfAcceptThreads = _ini.GetInteger(szServerTypeName, "ServerNumberOfAcceptThreads" );
		m_desc[0].dwNumberOfIoThreads = _ini.GetInteger(szServerTypeName, "ServerNumberOfIoThreads" );
		m_desc[0].dwNumberOfConnectThreads = _ini.GetInteger(szServerTypeName, "ServerNumberOfConnectThreads" );
		m_desc[0].dwMaxPacketSize = _ini.GetInteger(szServerTypeName, "ServerMaxPacketSize" );
		m_wPort[0] = _ini.GetInteger(szServerTypeName, "ServerListenPort" );
		m_byIOSize++;
	}

	szHandler = _ini.GetString(szServerTypeName, "ClientHandler", "false" );
	if (strcmp(szHandler.c_str(),"true")==0 ) {
		m_desc[1].dwSyncHandlerKey = CLIENT_SYNCHANDLER;
		m_desc[1].dwMaxAcceptSession = _ini.GetInteger(szServerTypeName, "ClientMaxAcceptSession" );
		m_desc[1].dwMaxConnectSession = _ini.GetInteger(szServerTypeName, "ClientMaxConnectSession" );
		m_desc[1].dwSendBufferSize = _ini.GetInteger(szServerTypeName, "ClientSendBufferSize" );
		m_desc[1].dwRecvBufferSize = _ini.GetInteger(szServerTypeName, "ClientRecvBufferSize" );
		m_desc[1].dwTimeOut = _ini.GetInteger(szServerTypeName, "ClientTimeOut" );
		m_desc[1].dwNumberOfAcceptThreads = _ini.GetInteger(szServerTypeName, "ClientNumberOfAcceptThreads" );
		m_desc[1].dwNumberOfIoThreads = _ini.GetInteger(szServerTypeName, "ClientNumberOfIoThreads" );
		m_desc[1].dwNumberOfConnectThreads = _ini.GetInteger(szServerTypeName, "ClientNumberOfConnectThreads" );
		m_desc[1].dwMaxPacketSize = _ini.GetInteger(szServerTypeName, "ClientMaxPacketSize" );
		m_wPort[1] = _ini.GetInteger(szServerTypeName, "ClientListenPort" );
		m_byIOSize++;
	}
}

void TemplateServerConfig::LoadConnectObjects( const char * szServerTypeName, UtilityIni & _ini )
{
	std::string szHandler="";

	szHandler = _ini.GetString(szServerTypeName, "ConnectToLoginServer", "false");
	if (strcmp(szHandler.c_str(),"true")==0 ) {
        m_pLoginSession = TemplateSessionFactory::Instance()->AllocLoginSession();
        m_pLobbySession->SetServerType( m_byServerType );
		LoadConnectObjectsAddress("LOGIN_SERVER", _ini, m_pLoginSession);
	}

	szHandler = _ini.GetString(szServerTypeName, "ConnectToAgentServer", "false");
	if (strcmp(szHandler.c_str(),"true")==0 ) {
        m_pAgentSession = TemplateSessionFactory::Instance()->AllocAgentSession();
        m_pAgentSession->SetServerType( m_byServerType );
		LoadConnectObjectsAddress("AGENT_SERVER", _ini, m_pAgentSession);
	}

	szHandler = _ini.GetString(szServerTypeName, "ConnectToLobbyServer", "false");
	if (strcmp(szHandler.c_str(),"true")==0 ) {
        m_pLobbySession = TemplateSessionFactory::Instance()->AllocLobbySession();
        m_pLobbySession->SetServerType( m_byServerType );
		LoadConnectObjectsAddress("LOBBY_SERVER", _ini, m_pLobbySession);
	}

	szHandler = _ini.GetString(szServerTypeName, "ConnectToGameServer", "false");
	if (strcmp(szHandler.c_str(),"true")==0 ) {
        m_pGameSession = TemplateSessionFactory::Instance()->AllocGameSession();
        m_pGameSession->SetServerType( m_byServerType );
		LoadConnectObjectsAddress("GAME_SERVER", _ini, m_pGameSession);
	}

	szHandler = _ini.GetString(szServerTypeName, "ConnectToDBServer", "false");
	if (strcmp(szHandler.c_str(),"true")==0 ) {
        m_pDBSession = TemplateSessionFactory::Instance()->AllocDBSession();
        m_pDBSession->SetServerType( m_byServerType );
		LoadConnectObjectsAddress("DB_SERVER", _ini, m_pDBSession);
	}
}

void TemplateServerConfig::LoadConnectObjectsAddress( const char * szServerTypeName,
		UtilityIni & _ini, TemplateServerSession * pServerSession )
{
	std::string szIP = _ini.GetString(szServerTypeName, "ServerAddress");
	WORD wPort = _ini.GetInteger(szServerTypeName, "ServerListenPort");
    pServerSession->SetAddr( (char*)szIP.c_str(), wPort );
    DEBUG_MSG( LVL_DEBUG, "%s = (%s : %d)", szServerTypeName, szIP.c_str(), wPort);
}

void TemplateServerConfig::Printf( SYNCHANDLER_DESC & desc )
{
    DEBUG_MSG( LVL_DEBUG, "MaxAcceptSession = %d", desc.dwMaxAcceptSession );
    DEBUG_MSG( LVL_DEBUG, "MaxConnectSession = %d", desc.dwMaxConnectSession );
    DEBUG_MSG( LVL_DEBUG, "SendBufferSize = %d", desc.dwSendBufferSize );
    DEBUG_MSG( LVL_DEBUG, "RecvBufferSize = %d", desc.dwRecvBufferSize );
    DEBUG_MSG( LVL_DEBUG, "TimeOut = %d", desc.dwTimeOut );
    DEBUG_MSG( LVL_DEBUG, "NumberOfAcceptThreads = %d", desc.dwNumberOfAcceptThreads );
    DEBUG_MSG( LVL_DEBUG, "NumberOfIoThreads = %d", desc.dwNumberOfIoThreads );
    DEBUG_MSG( LVL_DEBUG, "NumberOfConnectThreads = %d", desc.dwNumberOfConnectThreads );
    DEBUG_MSG( LVL_DEBUG, "MaxPacketSize = %d \n", desc.dwMaxPacketSize );
    DEBUG_MSG( LVL_DEBUG, "Port[0] = %d \n", m_wPort[0] );
    DEBUG_MSG( LVL_DEBUG, "Port[1] = %d \n", m_wPort[1] );
}


