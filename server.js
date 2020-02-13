var app = require('express')();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var bodyParser = require("body-parser");

var lampuLed = "";
var aksi = "";

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
  extended: true
}));


var socket2;
var ctr = 1;
//app.use(express.static('views'));

app.get('/', function (req, res) {
  res.sendFile(__dirname + '/views/UAS/index.html');
  //socket2.emit('news', { msg: req.query.BPM });	
});

app.get('/listening', function (req, res) {
  res.status(400).json({
    action: aksi,
    value: lampuLed
  })
});

app.post('/', function (req, res) {
  // console.log(req.body);
  // console.log(req.body);
  //lampuLed = req.body.toogleLed;
  io.sockets.emit('transmit', {
    msg: req.body
  });
});

// app.post('/coba', function (req, res) {
//   // res.status(201).json({
//   //   judul: req.body.message
//   // });
//   // console.log(req.body.message);
//   io.sockets.on('led', function (data) {
//     console.log(data.value);
//   });
// });

io.on('connection', function (socket) {
  socket2 = socket;
  io.sockets.emit('transmit', {
    msg: 'Connected to Device'
  });
  socket.on('led', function (data) {
    lampuLed = data.value;
    aksi = data.action;
    console.log(data);
  });
});

server.listen(5000, "159.65.138.36", function () {
  console.log("server started on port - 159.65.138.36:5000")
});