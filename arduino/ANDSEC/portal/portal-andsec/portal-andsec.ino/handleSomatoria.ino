void handleSomatoria(){

 if ( (server.arg("answer") == "9831892672") and logged_in) {

  fase2 = true;
  fase1 = false;
  
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent("<html><head></head><body>");
  ShowMenu();
  if (somatoria_answered == false ) pontos_atual+=200;
  server.sendContent("<h2> Congratulations, you pass.</h2> <br> Take your <b>serial number.</b><br><br> <a href='/admin'> continue </a>");

  Serial.print(
      " "
      "Serial Number: SDJIQyNyb2Nrcw=="
      ""
      ""
      );
 }
 else{
            server.sendHeader("Location", "/admin", true);
            server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
            server.sendHeader("Pragma", "no-cache");
            server.sendHeader("Expires", "-1");
            server.send ( 302, "text/plain", "");  // Empty content inhibits Content-length header so we have to close the socket ourselves.
            server.client().stop(); // Stop is needed because we sent no content length 
 }
  server.client().stop(); // Stop is needed because we sent no content length
  somatoria_answered = true;
}

