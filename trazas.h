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
  int envios_finalizados;
};
