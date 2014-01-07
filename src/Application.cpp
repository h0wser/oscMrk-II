#include "Application.h"

#include <iostream>

Application::Application()
	:
	mWindow(sf::VideoMode(1280, 720), "OscMrk-II", sf::Style::Close),
	mTextures(),
	mFonts(),
	mMouse(&mWindow),
	mStateStack(State::Context(mWindow, mTextures, mFonts, mMouse)),
	mFrameCount(0),
	mFrameTimer(),
	mFrameCountText()
{
	mWindow.setMouseCursorVisible(false);
}

void Application::run()
{
	loadResources();
	registerStates();

	sf::Clock dt;
	sf::Time lag;
	sf::Time updateTime = sf::seconds(1.f/60.f);

	while (mWindow.isOpen())
	{
		updateFrameCount(dt.getElapsedTime());

		lag += dt.getElapsedTime();
		dt.restart();

		handleEvents();

		while (lag >= updateTime)
		{
			update(updateTime);
			lag -= updateTime;
		}
		draw();
	}
}

void Application::update(sf::Time dt)
{
	mStateStack.update(dt);
	if (mStateStack.isEmpty())
	{
		mWindow.close();
	}
	mMouse.update(dt);
}

void Application::draw()
{
	mWindow.clear(sf::Color::Black);

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mFrameCountText);
	mWindow.draw(mMouse);
	mWindow.display();
}

void Application::handleEvents()
{
	sf::Event e;
	while (mWindow.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
		{
			mWindow.close();
		}
		mStateStack.handleEvent(e);
		mMouse.handleEvent(e);
	}
}

void Application::updateFrameCount(sf::Time dt)
{
	mFrameCount++;
	mFrameTimer += dt;
	if (mFrameTimer.asSeconds() >= 1.0f)
	{
		mFrameCountText.setString("FPS: " + std::to_string(mFrameCount));
		mFrameCount = 0;
		mFrameTimer = sf::Time::Zero;
	}
}

void Application::registerStates()
{
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<InventoryState>(States::Inventory);
	mStateStack.pushState(States::Title);
}

void Application::loadResources()
{
	try 
	{
		// Textures
		mTextures.loadTexture(Textures::Titlescreen, "resources/graphics/titlescreen.png");
		mTextures.loadTexture(Textures::Inventory, "resources/graphics/inventoryyo.png");
		mTextures.loadTexture(Textures::Tilesheet, "resources/graphics/map/tileset.png");
		mTextures.loadTexture(Textures::Button, "resources/graphics/gui/baseButton.png");
		mTextures.loadTexture(Textures::Player, "resources/graphics/player/player.png");
		mTextures.loadTexture(Textures::MouseClick, "resources/graphics/gui/cursor_click.png");
		mTextures.loadTexture(Textures::Mouse, "resources/graphics/gui/cursor.png");	
		mTextures.loadTexture(Textures::InventorySlot, "resources/graphics/gui/inventorySlot.png");
		mTextures.loadTexture(Textures::StackManager, "resources/graphics/gui/stackManager.png");
		mTextures.loadTexture(Textures::Tooltip, "resources/graphics/gui/tooltip.png");
		mTextures.loadTexture(Textures::DeleteItem, "resources/graphics/gui/deleteItem.png");
#pragma region Mob Textures
		mTextures.loadTexture(Textures::TestMob, "resources/graphics/Mob/test.png");
		mTextures.loadTexture(Textures::TestMobDead, "resources/graphics/Mob/dead.png");
#pragma endregion

#pragma region Item Textures
		mTextures.loadTexture(Textures::Test, "resources/graphics/item/test.png");
		mTextures.loadTexture(Textures::TestWeapon, "resources/graphics/item/testWeapon.png");

		mTextures.loadTexture(Textures::d_TestWeapon, "resources/graphics/player/gear/TestWeapon.png");
#pragma endregion

		// Fonts
		mFonts.loadFont(Fonts::Main, "resources/fonts/00TT.TTF");

		// Debug info
		mFrameCountText.setPosition(10, 10);
		mFrameCountText.setColor(sf::Color::White);
		mFrameCountText.setCharacterSize(16);
		mFrameCountText.setFont(*mFonts.getFont(Fonts::Main));
	}
	catch (std::runtime_error e)
	{
		std::cout << "EXCEPTION: " << e.what() << std::endl;
		exit(-1);
	}

	mMouse.setTextures(&mTextures);

}