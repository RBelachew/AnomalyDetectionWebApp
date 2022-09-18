# Anomaly Detection Server
      About Our Server
Our server is designed to calculate and display flight data anomalies that the client uploads. How does it happen?\
The client uploads 2 CSV flight files when the first is intended for the server learning (without anomalies)
and the another file is a real flight file that may have anomalies for which the client requests that we check for anomalies.
The server will display for which data an anomaly was discovered and at what time.

In addition, before the client sends his request he selects according to which algorithm he will want the server to consider anomalies.\
The user selects an algorithm for detecting anomalies from a drop-down list that includes an based algorithm regression and hybrid algorithm.

To get the calculation results the customer has to click on "submit" button to get the output.

<img src="https://user-images.githubusercontent.com/49268743/121658162-ba5ef080-caa9-11eb-9efa-b548763218ae.png">

    Directory Hierarchy
Model:\
      *getAnomalies.js - a js file which connects to the server of anomalies detector and return to controller the output.\
      *A folder called DetectorHTTPServer-main which contains (the inner server project of anomalies detector):\
	-Server.cpp - the file the client must run in order to get the anomalies to the controller and then to the browser/client.\
      -A folder which calls detectorFiles which contains (helper files):\
		anomaly_detection_util.cpp/.h\
		AnomalyDetector.h\
		CLI.cpp/.h\
		commands.h\
		HybridAnomalyDetector.cpp/.h\
		minCircle.cpp/.h\
		SimpleAnomalyDetector.cpp/.h\
		timeseries.cpp/.h\
      -A library which calls mongoose which contains (library files):\
		mongoose.c/.h
   
View:\
  index.html - a html file that holds all the visibility of our server.\
  index.css - a css file that holds all the visibility of our server.\
  index.ejs - the file which presents the anomalies in table to the client.\
  
Controller:\
  expServer.js - a Javascript file which used as a server and get requests from client and sends them to model as needed
  and vice versa, sends calculations he receives from the model (the anomalies) to the view.\

    Installation and Running Instructions
As a client of our server if you want to run the server you need to browse with http://localhost:8080/  .\
In addition you need to run the inner server which calculates the anomalies after sending the appropriate data, so let's get some order:\
At first,you need to run the Server.cpp file which exists in Model/DetectorHTTPServer-main folder.
There, you need to type a number of port,in example 3030 (this is the defult port) and after that,you can upload your data in the main server
of http://localhost:8080/ .

As a developer you need an IDE suitable for running Web Application.
In addition, you need some installations:
1. install Node.js with https://nodejs.org/en/download/
2. install npm with https://www.npmjs.com/get-npm
3. install some modules and packages with the following commands in cmd, when the working directory is the ~\AnomalyDetectionWebApp\Controller:\
  $ npm install express --save\
  $ npm i express-fileupload\
  $ npm install xmlhttprequest

<br>

    Documentation/UML
There is a link to UML charts of the main classes https://lucid.app/lucidchart/8d5ab3a0-95f6-4fae-9831-047a0e0129cd/view?page=0_0#https://lucid.app/lucidchart/8d5ab3a0-95f6-4fae-9831-047a0e0129cd/view?page=0_0%23 .Of course for developers there is also documentation of the code in the code files.

    Video
There is a link to a video where we demonstrate the use of the server:  https://youtu.be/IBIYnGvgxbM

