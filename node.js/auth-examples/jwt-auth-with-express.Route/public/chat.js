chat = (nameParam) => {

	$(function(){
		//make connection
	 var socket = io.connect('https://hp')
 
	 // buttons and inputs
	 var message = $("#message")
	 var username = $("#username")
	 var send_message = $("#send_message")
	 var send_username = $("#send_username")
	 var chatroom = $("#chatroom")
	 var feedback = $("#feedback")
 
	 //Emit message
	 send_message.click(function(){
		 socket.emit('message', {message : message.val()})
	 })
 
	 //Listen on new_message
	 socket.on("message", (data) => {
		 feedback.html('');
		 message.val('');
		 chatroom.append("<p class='message'>" + data.username + ": " + data.message + "</p>")
	 })
 
	 //Emit a username
	 send_username.click(function(){
		 socket.emit('username', {username : username.val()})
	 })
 
	 //Emit typing
	 message.bind("keypress", () => {
		 socket.emit('typing')
	 })
 
	 //Listen on typing
	 socket.on('typing', (data) => {
		 feedback.html("<p><i>" + data.username + " is typing a message..." + "</i></p>")
	 })

	 // Aqui esta a jogada para setar o username na instancia do Chat.
	 socket.emit('username', {username : nameParam})

	// ${message}.focus()
 })
 
 

}
