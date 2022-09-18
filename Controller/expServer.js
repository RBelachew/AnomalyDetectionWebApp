//requires
const express = require('express')
const fileUpload = require('express-fileupload')
const { each } = require('jquery')
const model = require('../Model/getAnomalies')


//set up
const app = express()
app.use(express.urlencoded({
    extended: false
}))
app.use(fileUpload())
app.use(express.static('../View'))
app.use('/jquery', express.static(__dirname + '/node_modules/jquery/dist/'));

app.set('view engine', 'ejs');

//http get - present the web page
app.get("/", (req, res) => {
   res.sendFile('index.html' , { root : __dirname});
})

//http post request - calculates anomalies
app.post("/detect", (req, res) => {
  //the selected algorithm
  let algo = req.body.algos.toString()
  if(req.files) {
      //extract the two files of client
      let file1 = req.files.normalCsv
      let file2 = req.files.anomalCsv
      //convert the two files to string 
      var normalData = file1.data.toString() //anotherMethod
      var anomalData = file2.data.toString() //anotherMethod

      //call to model server which calculates the anomalies with the appropriate params.
      //the callback is invoked AFTER the response arrives
      model.getAnomalies(normalData,anomalData,algo,
      function () {
          //the data arrive and send to view
          const data = JSON.parse(this.responseText);
          res.render('../../View/index', {data:data});
          res.end();
      })
    }
    else{
	    res.write('Error in files!\n')
      res.end()
    }
})

//listen port
app.listen(8080)
