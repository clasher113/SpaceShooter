#include "State.hpp"
#include "Application.hpp"

State::State(Application& application) :
	m_p_application(&application),
	m_p_assets(application.getAssets())
{
}

State::~State() {

}