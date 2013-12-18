                                Proyecto de Simulación

1. Estructura de la red
========================

Se pretende crear la siguiente estructura:


|   cacceso0    |           | cacceso1 |

=================           ============
|       |       |           |    |     |
c0n0   c0n1 ... |           |   c1n0   c1n1 ...
                =============
                  |       |
                 (r0)    (r0)

Donde c0 y c1 son instancias de ns3::NodeContainer, y representan, respectivamente, a la red de
una empresa y a internet. Cada red puede contener tantos nodos como se desee. Para unir dichas
redes, tenemos los routers r0 y r1. 

Desde la empresa se accederán a varios servicios de internet (Navegación web, descarga de archivos,
servicios VoIP). Por su parte, desde internet se accederá también a servicios internos de la 
empresa (simulando accesos de empleados trabajando desde casa, llamadas...)

Condiciones:


2. Cajon desastre
=============
- Se puntúa la complejidad:
  · Escenario
  · Modelos de fuentes
  · Amplitud resultados obtenidos
  · Modelar alto número de servicios: Correo, Web

- Fuentes a usar:
  · BulkSend: Tcp contínuo. Adecuado para transferencias de fichero
  · On/Off:   Tcp On (segundos), Off (Segundos). Adecuado para web
  · Sender:   Por determinar

- Ejemplos: /src/csma/examples: Base del trabajo oara la topología de red.


3. Por hacer.
=============
- Routers: Conseguir reenvío.
- ErrorModel
- Broadcast
- El número de nodos se tiene que poder establecer desde línea de comandos. Parametrizar todo lo posible.
- Preguntar formato de la memoria
- Pasar routers a ppp.
- Wifi: http://www.nsnam.org/doxygen-release/wifi-example-sim_8cc_source.html

4. Histórico
============
- Tenemos que añadir el módulo csma y csma-layout a .ns3rc

