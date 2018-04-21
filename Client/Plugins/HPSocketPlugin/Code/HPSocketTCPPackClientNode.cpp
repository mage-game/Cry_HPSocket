#include "StdAfx.h"
#include <CryFlowGraph/IFlowBaseNode.h> 
#include "HPSocketTCPPackClientWrapper.h"

namespace HPSocket
{
    class CTCPPackClientNode :public CFlowBaseNode<eNCT_Singleton>
    {
        enum INPUTS
        {
            EIP_Start = 0,
            EIP_Stop,
            EIP_Async,
        };

        enum OUTPUTS
        {
            EOP_Started = 0,
            EOP_Stoped,
        };
    public:
        CTCPPackClientNode(SActivationInfo* pActInfo)
        {
			m_pTCPPackClientWrapper = mEnv->pTCPPackClientWrapper.get();
        }

        virtual void GetMemoryUsage(ICrySizer* s) const
        {
            s->Add(*this);
        }

        virtual void GetConfiguration(SFlowNodeConfig& config)
        {
            static const SInputPortConfig in_config[] = {
                InputPortConfig_Void("Start"),
                InputPortConfig_Void("Stop"),
                InputPortConfig<bool>("Async",true),
                { 0 }
            };
            static const SOutputPortConfig out_config[] = {
                OutputPortConfig_Void("Started"),
                OutputPortConfig_Void("Stoped"),
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
			if (event== eFE_Activate)
			{
				if (IsPortActive(pActInfo, EIP_Start))
				{
					if (IConsole* pConsole = gEnv->pConsole)
					{
						float interval = pConsole->GetCVar("hp_socket_interval")->GetFVal();
						m_pTCPPackClientWrapper->SetDataFrameInterval(interval);

						string ip = pConsole->GetCVar("hp_socket_ip")->GetString();
						int port = pConsole->GetCVar("hp_socket_port")->GetIVal();
						bool async = GetPortBool(pActInfo, EIP_Async);
						if (!m_pTCPPackClientWrapper->Start(ip.c_str(), (USHORT)port, async))
						{
							LogError("Start client failed");
							return;
						}

						pActInfo->pGraph->SetRegularlyUpdated(pActInfo->myID, true);

						LogAlways("Start client succ,ip %s,port %d,interval %f", ip, port, interval);  
					}
				}
				else if (IsPortActive(pActInfo, EIP_Stop))
				{
					if (!m_pTCPPackClientWrapper->Stop())
					{
						LogError("Stop client failed");
						return;
					}

					ActivateOutput(pActInfo, EOP_Stoped, SFlowSystemVoid());
					LogAlways("Stop client succ");
				}
			}
			else if (event == eFE_Update)
			{
				if (m_pTCPPackClientWrapper->IsFinishHandShake())
				{
					pActInfo->pGraph->SetRegularlyUpdated(pActInfo->myID, false);
					ActivateOutput(pActInfo, EOP_Started, SFlowSystemVoid());
				}
			}
        }
    private:
		CTCPPackClientWrapper* m_pTCPPackClientWrapper;
    };      

    REGISTER_FLOW_NODE("HP-Socket:TCPPackClient", CTCPPackClientNode);
}
