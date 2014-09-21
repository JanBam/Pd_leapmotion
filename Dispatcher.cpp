#include "m_pd.h"
#include "Dispatcher.h"

using namespace Leap;

void Dispatcher::onInit(const Controller& controller) {
    post("Leap Motion for Pd by Chikashi Miyama & Jan Baumgart Ver 0.2");
}

void Dispatcher::onConnect(const Controller& controller) {
    post("Leap:connectd");
}

void Dispatcher::onDisconnect(const Controller& controller) {
    post("Leap:disconnected");
}

void Dispatcher::onExit(const Controller& controller) {
    post("Leap:Exited");
}

void Dispatcher::onFrame(const Controller& controller) {
    frame = controller.frame();
}

void Dispatcher::onFocusGained(const Controller& controller) {
    post("Leap:FocusGained");
}

void Dispatcher::onFocusLost(const Controller& controller) {
    post("Leap:FocusLost");
}

void Dispatcher::onDeviceChange(const Controller& controller) {
    post("Leap:DeviceChange");
}

void Dispatcher::onServiceConnect(const Controller& controller) {
    post("Leap:ServiceConnect");
}

void Dispatcher::onServiceDisconnect(const Controller& controller) {
    post("Leap:ServiceDisconnect");
}
