#include<iostream>
#include<crow.h>
#include<boost/filesystem.hpp>
#include<cstdlib>
#include<vector>
#include<fstream>

//bson imports 
#include<bsoncxx/json.hpp>
#include<bsoncxx/oid.hpp>
#include<bsoncxx/builder/stream/document.hpp>

//mongodb imports 
#include<mongocxx/client.hpp>
#include<mongocxx/stdx.hpp>
#include<mongocxx/uri.hpp>
#include<mongocxx/instance.hpp>

using  bsoncxx::builder::stream::close_array;
using  bsoncxx::builder::stream::close_document;
using  bsoncxx::builder::stream::document;
using  bsoncxx::builder::stream::finalize;
using  bsoncxx::builder::stream::open_array;
using  bsoncxx::builder::stream::open_document;
using  bsoncxx::builder::basic::kvp;
using  mongocxx::cursor;

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
	
	//connection to the database
	mongocxx::instance inst{};
	mongocxx::uri uri("mongodb://localhost:27017"); //connection string
	mongocxx::client conn(uri) ;

	auto collection = conn["cpp_db"]["contacts"];

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


	//contacts route
	CROW_ROUTE(app, "/contacts")([&collection]() {
		mongocxx::options::find opts;
		opts.limit(2);
		auto docs = collection.find({}, opts);

		ostringstream os;
		for (auto &&doc : docs)
			os << bsoncxx::to_json(doc) << " \n";

		return response(os.str());
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
	
	cout << "The server is up and running " << endl;
	//app listening to the port 
	app.port(PORT).multithreaded().run();
}