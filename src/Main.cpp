#include<iostream>
#include<crow.h>

using namespace std;
using namespace crow;

int main() {

	//initializing crow server 
	SimpleApp app;

	CROW_ROUTE(app, "/")([](const request &req,response &res) {
		
		ifstream getFile("public/index.html", ifstream::in); //finding the index.html file 
		if (getFile) {
			ostringstream contents;
			contents << getFile.rdbuf(); //extracting the contents of the file 
			getFile.close();
			//writting the response to the server
			res.write(contents.str());
		}
		else {
			res.write("Internal sever error ! File not found ");
		}

		res.end(); //ending the response from the server 
	});

	app.port(3000).multithreaded().run();
	
}