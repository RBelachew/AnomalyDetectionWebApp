#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <cstring>
#include <iostream>
#include <thread>
#include <sstream>
#include "mongoose/mongoose.h"
#include "detectorFiles/SimpleAnomalyDetector.h"
#include "detectorFiles/HybridAnomalyDetector.h"
#include "detectorFiles/timeseries.h"

static struct mg_serve_http_opts s_http_server_opts;
using namespace std;

//struct of 2 files and an algorithm
struct Files {
	std::string file1;
	std::string file2;
	std::string algo;
};
//extract the inputs to Files struct 
Files extractData(char* buf) {
	Files files;
	files.file1 = strtok(buf, "*");
	files.file2 = strtok(NULL, "*");
	files.algo = strtok(NULL, "*");
	return files;
}

//convert the vector of AnomalyReport to string according to format of JSON
std::string convertVecrtorToJASON(std::vector<AnomalyReport>& reports) {
	std::stringstream json;
	json << "[" << endl;
	for (auto it = reports.begin(); it < reports.end(); ++it) {
		json << "	{" << std::endl;
		json << "		\"timeStep\":" << it->timeStep << "," << endl;
		json << "		\"columns\":" << "\"" << it->description << "\"" << endl;
		json << "	}";
		if (it < reports.end() - 1) { json << ","; }
		json << endl;
	}
	json << "]";
	return json.str();
}

static void ev_handler(struct mg_connection* nc, int ev, void* p) {
	//if there is a client calculate the anomalies
	if (ev == MG_EV_HTTP_REQUEST) {
		struct http_message* test= (struct http_message*)p;
		char* data= new char[strlen((test->body.p)) + 1];
		strcpy(data, test->body.p);
		//order the data into Files struct
		Files files = extractData(data);
		//match the algorithm according to the client selection
		TimeSeriesAnomalyDetector* detector;
		if (files.algo.compare("hybrid")) {
			detector = new HybridAnomalyDetector();
		}
		else {
			detector = new SimpleAnomalyDetector();
		}
		std::thread l([files, detector]() -> void {
			TimeSeries t(files.file1.c_str());
			detector->learnNormal(t); });
		TimeSeries ts(files.file2.c_str());
		l.join();
		//return the vector of AnomalyReport 
		std::vector<AnomalyReport> v = detector->detect(ts);
		//convert the vector to string according to format of JSON
		string s = convertVecrtorToJASON(v);
		//sent the result to client
		cout << "Sent now" << endl;
		struct http_message* hm = (struct http_message*)p;

		mg_printf(nc, "HTTP/1.1 200 OK\r\nContent-Type: "
			"application/json\r\nContent-Length: %d\r\n\r\n%s",
			(int)strlen(s.c_str()), s.c_str());

		delete[] data;
		delete detector;
	}
}

int initServer(int port) {
	struct mg_mgr mgr;
	struct mg_connection* nc;
	std::string portToChar = std::to_string(port);
	static char const *sPort = portToChar.c_str();
	mg_mgr_init(&mgr, NULL);
	std::cout << "Starting web server on port " << sPort << std::endl;
	//bind with the ev_handler func
	nc = mg_bind(&mgr, sPort, ev_handler);
	if (nc == NULL) {
		std::cout << "failed to create listener" << std::endl;
	}
	mg_set_protocol_http_websocket(nc);
	s_http_server_opts.document_root = ".";
	s_http_server_opts.enable_directory_listing = "yes";
	//listen to client
	while (true) {
		cout << "listen" << endl;
		mg_mgr_poll(&mgr, 1000);
	}
	mg_mgr_free(&mgr);
	mg_mgr_free(&mgr);
	return 0;
}


int main(void) {
	int port;
	std::cout << "Select server port" << std::endl;
	std::cin >> port;
	if (std::cin.fail()) {
		port = 3030;
	}
	//init the server
	initServer(port);
	return 0;
}


