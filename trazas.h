# if 0
/*
  Trazas.h
  librería de la clase Trazas
*/
class Trazas
{
public:

  Trazas ();

  virtual ~Trazas ();

  void EventoEnviarFin (Ptr<const Packet> paquete);

  int GetEnviosFin();




private:
  //Variable que cuenta los paquetes cuyo envío ha finalizado.
  int envia_L;
  int recibe_L;
  int envia_R;
  int envia_L;

};
#endif

