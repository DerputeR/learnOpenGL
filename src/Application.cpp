#include "Application.h"
#include "engine/EngineCore.h"

int main() {
	Engine::App app{};
	return app.Start();
}