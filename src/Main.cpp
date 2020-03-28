#include<iostream>
#include<crow.h>

using namespace std;
using namespace crow;


//function to send files
void sendFile(response &res, string filename, string contentType) {
	ifstream getFile("public/" + filename, ifstream::in);
	if (getFile) {
		ostringstream contents;
		contents << getFile.rdbuf();
		getFile.close();
		res.set_header("Content-Type", contentType);
		res.write(contents.str());
	}
	else {
		res.code = 404;
		res.write("Internal Server error , file not found");
	}
	res.end();
}

//function to send html
void sendHtml(response &res, string filename) {
	sendFile(res, filename + ".html", "text/html");
}

//helper function to send images
void sendImage(response &res, string filename) {
	sendFile(res, "images/" + filename, "image/jpeg");
}

//helper function to send scripts
void sendScripts(response &res, string filename) {
	sendFile(res, "js/" + filename, "text/javascript");
}

//helper function to send styles
void sendCss(response &res, string filename) {
	sendFile(res, "css/" + filename, "text/css");
}

int main() {

	//initializing crow server 
	SimpleApp app;

	CROW_ROUTE(app, "/css/<string>")([](const request &req, response &res, string filename) {
		sendCss(res, filename);
	});

	CROW_ROUTE(app, "images/<string>")([](const request &req, response &res, string filename) {
		sendImage(res, filename);
	});

	CROW_ROUTE(app, "/")([](const request &req,response &res) {
		sendHtml(res, "index");
	});

	app.port(3000).multithreaded().run();
	
}