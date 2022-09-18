//Requesting the calculation server for the anomalies
function getAnomalies(normalData,anomalData,algo,callback) {
	//set up for http request to the inner server
	var XMLHttpRequest = require('../Controller/node_modules/xmlhttprequest').XMLHttpRequest;
	var http = new XMLHttpRequest();
	var url = "http://localhost:3030";
	var params = normalData+"*"+anomalData+"*"+algo;
	http.open("POST", url, true);

	http.setRequestHeader("Content-Type", "application/json");

	http.onreadystatechange = function() {
		//if the request succeed it invoked the callback function
	    if(http.readyState == 4 && http.status == 200) {
    	     if (typeof callback === "function") {
                // apply() sets the meaning of "this" in the callback
                callback.apply(http);
            }
	    }
	}
	//send params to inner server
	http.send(params)
}
module.exports.getAnomalies = getAnomalies
