// servidor.cc

#include "servidor.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Servidor");

Servidor::Servidor(Ptr< Node > disp)
:Application(),m_node(PeekPointer(disp))
{
  //m_node = disp.Copy();
  DoInitialize();
}

#if 0
 void
Servidor::StartApplication (void)
{
 NS_LOG_FUNCTION (this);

 m_socket = Socket::CreateSocket (GetNode (), TypeId::LookupByName ("ns3::TcpSocketFactory"));
 NS_ASSERT (m_socket != 0);
 // m_socket->SetAttribute ("Protocol", UintegerValue (6)); // tcp
 m_socket->SetRecvCallback (MakeCallback (&Servidor::RecibePaquete, this));
 InetSocketAddress src = InetSocketAddress (Ipv4Address::GetAny (), 0);
 int status;
 status = m_socket->Bind (src);
 NS_ASSERT (status != -1);

 // InetSocketAddress dst = InetSocketAddress (m_remote, 0);
 // status = m_socket->Connect (dst);
 // NS_ASSERT (status != -1);

 // Send ();
}
#endif

/// Recibe la petición web. Se asume que es válida siempre.
void Servidor::RecibePaquete (Ptr<Socket> socket)
{
	NS_LOG_FUNCTION (this << socket);
	while (m_socket->GetRxAvailable () > 0)
	{
		Address from;
		Ptr<Packet> p = m_socket->RecvFrom (0xffffffff, 0, from);
		NS_LOG_DEBUG ("recv " << p->GetSize () << " bytes");
		NS_ASSERT (InetSocketAddress::IsMatchingType (from));
		InetSocketAddress realFrom = InetSocketAddress::ConvertFrom (from);
		NS_ASSERT (realFrom.GetPort ()); // assure port 80 or 443
		// Just if we want do stuff with this
		// Ipv4Header ipv4;
		// p->RemoveHeader (ipv4);
		// uint32_t recvSize = p->GetSize ();
		// NS_ASSERT (ipv4.GetProtocol () == 6); // protocol should be tcp.
		// TcpHeader tcp; // If we want do stuufs
		// p->RemoveHeader (tcp);

		RespondePaquete(realFrom.GetIpv4());
	}
}

/// Método que envía un nuevo paquete.
/// Deberá iniciar el temporizador de retransmisiones
void Servidor::RespondePaquete(const Ipv4Address &desde)
{
	const char cadena_prueba[] = "hola";
	Ptr<Packet> paquetePrueba = new Packet((uint8_t *)cadena_prueba,strlen(cadena_prueba),true);
	//Ptr<Packet> enviar = new Packet(1); // TODO Tamaño aleatorio
	
	m_socket->SendTo(paquetePrueba,0,desde);
}

// Método de inicialización de la aplicación. Instala el Callback.
void Servidor::DoInitialize()
{
 NS_LOG_FUNCTION (this);

 m_socket = Socket::CreateSocket (m_node, TypeId::LookupByName ("ns3::TcpSocketFactory"));
 NS_ASSERT (m_socket != 0);
 // m_socket->SetAttribute ("Protocol", UintegerValue (6)); // tcp
 m_socket->SetRecvCallback (MakeCallback (&Servidor::RecibePaquete, this));
 InetSocketAddress src = InetSocketAddress (Ipv4Address::GetAny (), 0);
 int status;
 status = m_socket->Bind (src);
 NS_ASSERT (status != -1);
 status = m_socket->Listen();

 // InetSocketAddress dst = InetSocketAddress (m_remote, 0);
 // status = m_socket->Connect (dst);
 // NS_ASSERT (status != -1);

 // Send ();

  //GetNode()->RegisterProtocolHandler (ns3::MakeCallback(&Servidor::RecibePaquete,
  //                                                     this),
  //                                   0x0800, 0, false);
//
  Application::DoInitialize();
}
