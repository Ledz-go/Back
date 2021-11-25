# Master Node
The Master Node is the one parsing the message from the Smartphone and relaying the appropriate data to the different nodes it is connected to. It has to initate a WiFi hot spot and receive WiFi connection requests from all the surrounding nodes.

## Communication Layer
TBD

## Application Layer
In the application layer we will open sockets between the different agents of the network and manage the data processing. It is a Stateless microcontroller.
In order the ESP8266 will:
1. Receive JSON message
2. Interpret the JSON message into a command loop
3. Send JSON messages to slave-nodes

#### Authentication and Connection Management
TBD : Do we need a password to connect to the Master Node? SessionId creation for the connection, to be used for log messages. System status must be kept up to date and analysed

#### To Do
- [ ] Finite State Machine for the in messages
- [ ] Finite State Machine for the out messages
- [ ] API for the smartphone message processing
- Pas de ACK, pas de message de retour
- Transfomrer la librairie des lumières en un seul JSON
- Interogation du réseau



