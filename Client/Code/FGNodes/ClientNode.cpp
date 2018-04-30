#include "StdAfx.h"
#include <CryFlowGraph/IFlowBaseNode.h> 
#include <HPSocketPluginApi/HPSocketSerializer.h>
#include "HPSocketPluginProxy.h"
#include "PkgBody.h"

const float SEND_PERIOD = 2.0f;

class CTCPPackClientLoginNode :public CFlowBaseNode<eNCT_Singleton>
{
	enum INPUTS
	{
		EIP_Active = 0,
	};

	enum OUTPUTS
	{
		EOP_Done = 0,
		EOP_Successed,
		EOP_Failed,
	};
public:
	CTCPPackClientLoginNode(SActivationInfo* pActInfo) {}
	~CTCPPackClientLoginNode() {}

	virtual void GetMemoryUsage(ICrySizer* s) const
	{
		s->Add(*this);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig in_config[] = {
			InputPortConfig<bool>("Active",true),
			{ 0 }
		};
		static const SOutputPortConfig out_config[] = {
			OutputPortConfig_Void("Done"),
			OutputPortConfig_Void("Successed"),
			OutputPortConfig_Void("Failed"),
			{ 0 }
		};

		config.sDescription = _HELP("");
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		//config.nFlags |= EFLN_TARGET_ENTITY;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		if (event == eFE_Activate)
		{
			if (IsPortActive(pActInfo, EIP_Active))
			{
				m_lastTickTime = gEnv->pTimer->GetFrameStartTime();
				pActInfo->pGraph->SetRegularlyUpdated(pActInfo->myID, true);

				HPSocket::Serializer serializer;
				serializer.Serialize(LOCAL_PLAYER_LOGIN);
				bool isSendSuccessed = mEnv->g_pHPSocketPluginProxy->SerializeSinglePkgDirectly(serializer);
				if (!isSendSuccessed)
				{
					ActivateOutput(pActInfo, EOP_Failed, SFlowSystemVoid());
				}
				ActivateOutput(pActInfo, EOP_Done, SFlowSystemVoid());
			}
		}
		else if (event == eFE_Update)
		{
			CTimeValue time = gEnv->pTimer->GetFrameStartTime();
			CTimeValue dt = time - m_lastTickTime;
			if (dt.GetSeconds() < SEND_PERIOD)
			{
				uint64_t connID = mEnv->g_pGameObjectCreatHelper->GetLocalPlayerConnID();
				if (connID != 0)
				{
					pActInfo->pGraph->SetRegularlyUpdated(pActInfo->myID, false);
					ActivateOutput(pActInfo, EOP_Successed, SFlowSystemVoid());
				}
			}
			else
			{
				pActInfo->pGraph->SetRegularlyUpdated(pActInfo->myID, false);
				ActivateOutput(pActInfo, EOP_Failed, SFlowSystemVoid());
			}
		}
	}
private:
	CTimeValue m_lastTickTime;
};

class CTCPPackClientSelectActorNode :public CFlowBaseNode<eNCT_Singleton>
{
	enum INPUTS
	{
		EIP_Active = 0,
		EIP_ActorID,
	};

	enum OUTPUTS
	{
		EOP_Successed = 0,
		EOP_Failed,
	};
public:
	CTCPPackClientSelectActorNode(SActivationInfo* pActInfo) {}
	~CTCPPackClientSelectActorNode() {}

	virtual void GetMemoryUsage(ICrySizer* s) const
	{
		s->Add(*this);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig in_config[] = {
			InputPortConfig_Void("Active"),
			InputPortConfig<int>("ActorId",0),
			{ 0 }
		};
		static const SOutputPortConfig out_config[] = {
			OutputPortConfig_Void("Successed"),
			OutputPortConfig_Void("Failed"),
			{ 0 }
		};

		config.sDescription = _HELP("");
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		//config.nFlags |= EFLN_TARGET_ENTITY;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		if (event == eFE_Activate)
		{
			if (IsPortActive(pActInfo, EIP_Active))
			{
				ActorInfo actorInfo;
				uint64_t connID = mEnv->g_pGameObjectCreatHelper->GetLocalPlayerConnID();
				string name = "Player_" + connID;

				actorInfo.actorID = GetPortInt(pActInfo, EIP_ActorID);
				memcpy(actorInfo.name, name.c_str(), sizeof(name));

				if (mEnv->g_pGameObjectCreatHelper->SelectLocalActorTemplate(actorInfo))
				{
					PkgBodySelectActorInfo pkgBodySelectActorInfo;
					pkgBodySelectActorInfo.actor = actorInfo;

					HPSocket::Serializer serializer;	 					
					serializer.Serialize(SELECT_ACTOR_INFO, pkgBodySelectActorInfo);
					bool isSendSuccessed = mEnv->g_pHPSocketPluginProxy->SerializeSinglePkgDirectly(serializer);
					if (isSendSuccessed)
					{
						ActivateOutput(pActInfo, EOP_Successed, SFlowSystemVoid());
						return;
					}
				}
				ActivateOutput(pActInfo, EOP_Failed, SFlowSystemVoid());
			}
		}
	}
};

class CTCPPackClientAllNetPlayerInfoNode :public CFlowBaseNode<eNCT_Singleton>
{
	enum INPUTS
	{
		EIP_Active = 0,
	};

	enum OUTPUTS
	{
		EOP_Successed = 0,
		EOP_Failed,
	};
public:
	CTCPPackClientAllNetPlayerInfoNode(SActivationInfo* pActInfo) {}
	~CTCPPackClientAllNetPlayerInfoNode() {}

	virtual void GetMemoryUsage(ICrySizer* s) const
	{
		s->Add(*this);
	}

	virtual void GetConfiguration(SFlowNodeConfig& config)
	{
		static const SInputPortConfig in_config[] = {
			InputPortConfig<bool>("Active",true),
		{ 0 }
		};
		static const SOutputPortConfig out_config[] = {
			OutputPortConfig_Void("Successed"),
			OutputPortConfig_Void("Failed"),
		{ 0 }
		};

		config.sDescription = _HELP("");
		config.pInputPorts = in_config;
		config.pOutputPorts = out_config;
		//config.nFlags |= EFLN_TARGET_ENTITY;
		config.SetCategory(EFLN_APPROVED);
	}

	virtual void ProcessEvent(EFlowEvent event, SActivationInfo* pActInfo)
	{
		static CTimeValue lastTickTime;

		if (event == eFE_Activate)
		{
			if (IsPortActive(pActInfo, EIP_Active))
			{
				HPSocket::Serializer serializer;
				serializer.Serialize(ALL_NET_PLAYER_INFO);
				bool isSendSuccessed = mEnv->g_pHPSocketPluginProxy->SerializeSinglePkgDirectly(serializer);
				if (isSendSuccessed)
				{
					ActivateOutput(pActInfo, EOP_Successed, SFlowSystemVoid());
					return;
				}
				ActivateOutput(pActInfo, EOP_Failed, SFlowSystemVoid());
			}
		}
	}
};

REGISTER_FLOW_NODE("HP-Socket:Login", CTCPPackClientLoginNode);
REGISTER_FLOW_NODE("HP-Socket:SelectActor", CTCPPackClientSelectActorNode);
REGISTER_FLOW_NODE("HP-Socket:GetAllNetPlayerInfo", CTCPPackClientAllNetPlayerInfoNode);

