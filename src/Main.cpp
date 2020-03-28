#include<iostream>
#include<crow.h>

using namespace std;
using namespace crow;

int main() {

	//initializing crow app
	SimpleApp app;

	CROW_ROUTE(app, "/")([](const request &req, response &res) {
		ifstream getFile("public/index.html",ifstream::in);
		if (getFile) {
			ostringstream contents;
			contents << getFile.rdbuf();
			getFile.close();
			res.write(contents.str());
			res.end();
		}
		else {
			res.write("Internal server error ! , File not found ");
		}

	});

	app.port(3000).multithreaded().run();

	return 0;
}