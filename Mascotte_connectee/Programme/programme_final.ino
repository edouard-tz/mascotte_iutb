// Charger les librairies
#include <WiFi.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include "SoundData.h";
#include "Game_Audio.h";

// son

Game_Audio_Class GameAudio(25,0); 
Game_Audio_Wav_Class pmWav(pacman); 


const char* ssid = "Mon-ESP32-en-AP";   // Identifiant WiFi
const char* password = "123456789";  // Mot de passe WiFi

WiFiServer server(80); // Définir le numéro de port du serveur web sur 80

String header;// Variable pour stocker la requête HTTP

String output13State = "off";// Variable auxiliaire pour mémoriser l'état de la sortie en cours
String output12State = "off";// Variable auxiliaire pour mémoriser l'état de la sortie en cours
String output27State = "off";

int pos = 0;//Position de départ des servomoteurs

// Création des objets servomoteurs
Servo monservo ;
Servo monservo2 ;

LiquidCrystal monEcran(27, 33, 15, 32, 14, 4); // création de l'objet écran



void setup() {
  Serial.begin(115200);
  
  // Fixe les servos sur les broche 12 et 13 aux objets créés juste au dessus
  monservo.attach(13);
  monservo2.attach(12);

  // Initialiser les moteurs à une position de départ
  monservo.write(pos);
  monservo2.write(pos);



  monEcran.begin(16, 2); //on initialise la communication avec 16 colonnes et deux lignes
  monEcran.clear(); // on efface l'écran (pour si jamais il reste des choses dessus)
  monEcran.setCursor(5, 0); //on place notre curseur à la 5ème colonne de la première ligne
  monEcran.print("Salut,"); // on écrit "Salut,"
  monEcran.setCursor(0, 1); //on passe à la première colonne la ligne suivante
  monEcran.print("Je suis Poulpo!"); // on écrit "Je suis Poulpo!"
  delay(3000); // délai de 3s.
  monEcran.clear(); // on efface l'écran
  monEcran.setCursor(0, 0); //on place notre curseur à la première colonne de la première ligne
  monEcran.print("Connecte-toi"); // on écrit "Connecte-toi"
  monEcran.setCursor(0, 1); //on passe à la ligne suivante
  monEcran.print("en WiFi sur :"); // on finit d'écrire "en WiFi sur :"
  delay(3000); // délai de  2s.
  monEcran.clear(); // on efface l'écran
  monEcran.setCursor(0, 0); //on place notre curseur à la première colonne de la première ligne
  monEcran.print("Mon-ESP32-en-AP"); // on écrit le ssid "Mon-ESP32-en-AP"
  monEcran.setCursor(0, 1); //on passe à la ligne suivante
  monEcran.print("mdp: 123456789"); // on écrit le mdp "123456789"
  delay(5000); // délai de 6s.
  monEcran.clear(); // on efface l'écran
  monEcran.setCursor(0, 0); //on place notre curseur à la première colonne de la première ligne
  monEcran.print("Adresse IP:"); // on écrit "Adresse IP:"
  monEcran.setCursor(0, 1); //on passe à la ligne suivante
  monEcran.print("192.168.4.1"); // on finit par écrire l'adresse IP "192.168.4.1"
  delay(5000);
  monEcran.clear();
  monEcran.setCursor(5, 0);
  monEcran.print("0    0");
  monEcran.setCursor(7, 1);
  monEcran.print("__");

  Serial.print("Setting AP (Access Point)..."); // Connection au réseau Wi-Fi avec SSID et mot de passe
  WiFi.softAP(ssid, password); // possibilité de supprimer le mot de apsse
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Adresse IP: ");
  Serial.println(IP);
  server.begin();// Lancement du serveur

}


void loop() {
  WiFiClient client = server.available();   // Être à l'écoute des clients entrants

  if (client) {                             // Si un nouveau client se connecte
    Serial.println("Nouveau Client");          // affiche un message sur le moniteur sérieport
    String currentLine = "";                // faire une chaîne de caractères pour contenir les données entrantes du client
    while (client.connected()) {            // boucle tant que le client est connecté
      if (client.available()) {             // s'il y a des octets à lire sur le client,
        char c = client.read();             // lire un octet, puis
        Serial.write(c);                    // l'affiche sur le moniteur série
        header += c;
        if (c == '\n') {                    // si l'octet est un caractère de nouvelle ligne
          // Si la ligne courante est vide, vous avez deux caractères de nouvelle ligne dans une ligne.
          // C'est la fin de la requête HTTP du client, alors envoyez une réponse :
          if (currentLine.length() == 0) {
            // Les en-têtes HTTP commencent toujours par un code de réponse (par ex. HTTP/1.1 200 OK).
            // et un type de contenu pour que le client sache ce qui s'en vient, puis une ligne blanche :
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
// --------------------------------------------------------------------------   
            // Moteur droit
            if (header.indexOf("GET /13/on") >= 0) {
              Serial.println("GPIO 13 on");
              monservo.write(180);
              output13State = "on";
              
            }
              else if (header.indexOf("GET /13/off") >= 0) {
                Serial.println("GPIO 12 off");
                monservo.write(0);
                output13State = "off";
                
              }
// --------------------------------------------------------------------------            
            // Moteur gauche
              else if (header.indexOf("GET /12/on") >= 0) {
                Serial.println("GPIO 12 on");
                monservo2.write(180);
                output12State = "on";
                
              }
              else if (header.indexOf("GET /12/off") >= 0) {
                Serial.println("GPIO 12 off");
                monservo2.write(0);
                output12State = "off";
                
              }
// --------------------------------------------------------------------------   
        // smiley
              else if (header.indexOf("GET /27/on") >= 0) {
                Serial.println("GPIO 27 on");
                
                monEcran.clear();
                monEcran.setCursor(5, 0);
                monEcran.print("^    ^");
                monEcran.setCursor(7, 1);
                monEcran.print("__");
                monservo.write(180);
                monservo2.write(0);
                output27State = "on";
      
              }
              else if (header.indexOf("GET /27/off") >= 0) {
                Serial.println("GPIO 27 off");
                monEcran.clear();
                monEcran.setCursor(5, 0);
                monEcran.print("x    x");
                monEcran.setCursor(7, 1);
                monEcran.print("__");
                monservo.write(0);
                monservo2.write(180);  
                GameAudio.PlayWav(&pmWav, false, 1.0);
                while(GameAudio.IsPlaying()){   }
                Serial.print("Son ok ");                   
                output27State = "off";
              }

// --------------------------------------------------------------------------  
            // Afficher la page Web HTML
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
            client.println("<link rel='icon' href=\"data:,\">");

            // CSS pour styliser les boutons mdes démos
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #c10534; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #1a2fa5;}border: none; color: white; padding: 16px 40px;</style></head>");

            // Titre de la page Web
            client.println("<body><h1>Serveur Mascotte</h1>");

// --------------------------------------------------------------------------   
            client.println("<p> Servomoteur droit : </p>");


            if (output13State == "off") {
              client.println("<p><a href='/13/on'><button class='button'>MONTER</button></a></p>");
            }
            else {
              client.println("<p><a href='/13/off'><button class='button button2'>BAISSER</button></a></p>");
            }
// --------------------------------------------------------------------------   
            client.println("<p> Servomoteur gauche : </p>");


            if (output12State == "off") {
              client.println("<p><a href='/12/on'><button class='button'>BAISSER</button></a></p>");
            }
            else {
              client.println("<p><a href='/12/off'><button class='button button2'>MONTER</button></a></p>");
            }

// --------------------------------------------------------------------------   

            client.println("<p> Humeur de poulpo : </p>");


            if (output27State == "off") {
              client.println("<p><a href='/27/on'><button class='button'>content</button></a></p>");
            }
            else {
              client.println("<p><a href='/27/off'><button class='button button2'>triste</button></a></p>");
            }

// --------------------------------------------------------------------------   
            client.println("</body></html>");

            // La réponse HTTP se termine par une autre ligne blanche
            client.println();
            // Sortir de la boucle de temps
            break;
          } else { //  si vous avez une nouvelle ligne, alors effacez currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si vous n'avez rien d'autre qu'un personnage de retour de chariot,
          currentLine += c;      // l'ajouter à la fin de la ligne courante
        }
      }
    }
    // Effacer la variable d'en-tête
    header = "";
    // Fermer la connexion
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
