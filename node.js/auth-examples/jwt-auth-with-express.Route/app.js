'use strict'
/**
 * To get started install
 * express bodyparser jsonwebtoken express-jwt
 * via npm
 * command :-
 * npm install express bodyparser jsonwebtoken express-jwt --save
 */
require('rootpath')()
// Bringing all the dependencies in
const express = require('express')
const bodyParser = require('body-parser')
const jwt = require('jsonwebtoken')
const exjwt = require('express-jwt')
const rotas = require('./routes')


// Instantiating the express app
const app = express()

// See the react auth blog in which cors is required for access
app.use((req, res, next) => {
  res.setHeader('Access-Control-Allow-Origin', 'http://192.168.15.3:8080')
  res.setHeader('Access-Control-Allow-Headers', 'Content-type,Authorization')
  next()
})

// Setting up bodyParser to use json and set it to req.body
app.use(bodyParser.json())
app.use(bodyParser.urlencoded({ extended: true }))

app.set('view engine', 'ejs')
// usando o layout do EJS

// INstantiating the express-jwt middleware
const jwtMW = exjwt({
  secret: 'keyboard cat 4 ever'
})

// MOCKING DB just for test
// LOGIN ROUTE
// app.post('/login', (req, res) => {
//     const { username, password } = req.body;
//     console.log('req.body=' + req.body)
//     // Use your DB ORM logic here to find user and compare password
//     for (let user of users) { // I am using a simple array users which i made above
//         if (username == user.username && password == user.password /* Use your password hash checking logic here !*/) {
//             //If all credentials are correct do this
//             let token = jwt.sign({ id: user.id, username: user.username }, 'keyboard cat 4 ever', { expiresIn: 129600 }); // Sigining the token
//             res.json({
//                 sucess: true,
//                 err: null,
//                 token
//             });
//             break;
//         }
//         else {
//             res.status(401).json({
//                 sucess: false,
//                 token: null,
//                 err: 'Username or password is incorrect'
//             });
//         }
//     }
// });

// app.get('/', jwtMW /* Using the express jwt MW here */, (req, res) => {
//    res.send('You are authenticated'); //Sending some response when authenticated
// });

// router.get('/', jwtCheckAuth, rootController.root)
app.use('/', express.static('public'))
app.use('/img', express.static('img'))

app.use(require('./routes'))

// Error handling
app.use(function (err, req, res, next) {
  if (err.name === 'UnauthorizedError') { // Send the error rather than to show it on the console
    res.status(401).redirect('/login')
    // res.status(401).send(err)
  } else {
    next(err)
  }
})

module.exports = app
