#include "SplashScreen.h"

namespace star
{
	SplashScreen::SplashScreen(const tstring& name, const tstring & nextScene)
		: SlideScene(name, nextScene)
	{

	}

	SplashScreen::~SplashScreen()
	{

	}

	void SplashScreen::AfterInitializedObjects()
	{
		AddSlide(
			_T("StarEngine/SESplashScreen.png"),
			5.0f,
			star::Color(0,0,0,0),
			star::Color::White,
			0.5f,
			star::Color::White,
			star::Color(0,0,0,0),
			0.75f
			);
	}
}