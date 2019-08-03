/*
 *ANC做了两件事：第一是把sc1接口的数据加上头转发给PCC，以及把PCC发来的加上头送给UE;另一件事情是和媒体网关控制器商量目的端口
 * */
#include "UDPSocket.h"
#include "ANC.h"
#include "pthread.h"
#include "unistd.h"

using namespace std;

unsigned short UEport = 50001;//这是ANC和UE通信的端口
unsigned short srcport = 50000;//这是ANC和PCC通信的端口
unsigned short destport = 50000;//这是ANC向PCC发送消息的目的端口
string PCCaddr = "162.106.85.198";
string UEaddr = "162.105.85.198";

void *UE_ANC(void*){

	UDPSocket AncUEsock;
	ANC Upanc;
	if(AncUEsock.create(UEport)<0)
    	{
            cout<<"AncUEsock error"<<endl;
        }
	else{
			
		cout<<"AncUEsock created"<<endl;
	
		char sigbuf[100];
		string srcip_;
		unsigned short sport_;
        
		while(1){//不断接收上行和下行的信号
			//UE给PCC的上行信号，需要发送给PCC的50001端口
			int lenrecv = AncUEsock.recvbuf(sigbuf,100,srcip_,sport_);
			cout<<"接收到UE上行的信号长度是： "<<lenrecv<<endl;

			char* toPcc = Upanc.UpaddHeader(sigbuf, lenrecv);
			int lensend = AncUEsock.sendbuf(toPcc, 8+lenrecv,PCCaddr, 50000);
			cout<<"发送给PCC的信号长度是： "<<lensend<<endl;

		}
	
	}
	AncUEsock.Close();
	
}

void *PCC_ANC(void*){

	UDPSocket PCCAncsock;
	ANC Downanc;
	if(PCCAncsock.create(srcport)<0)
    	{
            cout<<"PCCAncsock error"<<endl;
            
        }
	else{
	
		cout<<"PCCAncsock created"<<endl;
	
		char sigbuf[100];
		string srcip_;
		unsigned short sport_;
        
		while(1){//不断接收上行和下行的信号
			//PCC给ANC的下行信号，需要发送给ANC的50000端口
			int lenrecv = PCCAncsock.recvbuf(sigbuf,100,srcip_,sport_);
			cout<<"接收到PCC下行的信号长度是： "<<lenrecv<<endl;
		
			char* toUE = Downanc.DownaddHeader(sigbuf, lenrecv);
			int lensend = PCCAncsock.sendbuf(toUE, 8+lenrecv,UEaddr, 10002);
			cout<<"发送给UE的信号长度是： "<<lensend<<endl;

		}
	}
	PCCAncsock.Close();
}

int main()
{	
	pthread_t thid1, thid2;
	if( pthread_create(&thid1, NULL,  UE_ANC, NULL)!= 0){
		cout<<"UE_ANC thread create error"<<endl;
		return -1;
	}

	if( pthread_create(&thid2, NULL, PCC_ANC, NULL)!= 0){
		cout<<"PCC_ANC thread create error"<<endl;
		return -1;
	}
	pthread_join(thid1,NULL);
	pthread_join(thid2,NULL);

	return 0;
}

