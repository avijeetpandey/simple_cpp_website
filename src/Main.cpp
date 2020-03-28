#include<iostream>
#include<crow.h>

using namespace std;
using namespace crow;


void sendFile(response &res, string filename, string contentType) {
	//extracting the file
	ifstream getFile("public/" + filename, ifstream::in | ifstream::binary);

	if (getFile) {
		ostringstream contents;
		contents << getFile.rdbuf();  //loading the contents of the file
		getFile.close();
		res.set_header("Content-Type", contentType);
		res.write(contents.str());
	}
	else {
		res.code = 404;
		res.write("Internal server error , file not found ");
	}
	//end of the response 
	res.end();
}

void sendHTML(response &res, string filename) {

	sendFile(res, filename + ".html", "text/html");
}

void sendScripts(response &res, string filename) {
	sendFile(res, "scripts/" + filename, "text/javascript");
}

void sendImage(response &res, string filename) {
	sendFile(res, "images/" + filename, "image/jpeg");
}

void sendCSS(response &res, string filename) {
	sendFile(res, "css/" + filename, "text/css");
}

int main() {

	//initializing the sample app
	SimpleApp app;
	
	const int PORT = 3000;

	//sending the files to the server
	CROW_ROUTE(app, "/images/<string>")([](const request &req, response &res, string filename) {
	    sendImage(res, filename);
	});

	CROW_ROUTE(app, "/css/<string>")([](const request &req, response &res, string filename) {
		sendCSS(res, filename);
	});

	CROW_ROUTE(app, "/scripts/<string>")([](const request &req, response &res, string filename) {
		sendScripts(res, filename);
	});


	//home route
	CROW_ROUTE(app, "/")([](const request &req, response &res) {
		sendHTML(res,"index");
	});

	//about route
	CROW_ROUTE(app, "/about")([](const request &req, response &res) {
		sendHTML(res,"about");
	});

	//login route
	CROW_ROUTE(app, "/login")([](const request &req, response &res) {
		sendHTML(res,"login");
	});
	
	//app listening to the port 
	app.port(PORT).multithreaded().run();
}