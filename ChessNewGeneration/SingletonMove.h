#pragma once
#include <string>
#include <QWidget>

class SingletonMove : public QWidget {
	Q_OBJECT
	enum class State {
		WAITING_FOR_ORIGIN,
		WAITING_FOR_DESTINATION,
		READY
	};
	State state = State::WAITING_FOR_ORIGIN;
	SingletonMove() {}
	void clear() {
		state = State::WAITING_FOR_ORIGIN;
	}
public:
	std::string originPosition;
	std::string destinationPosition;

	static SingletonMove* getSingleton() {
		static SingletonMove sm;
		return &sm;
	}

	void setPosition(const std::string& position) {
		if (state == State::WAITING_FOR_ORIGIN) {
			originPosition = position;
			state = State::WAITING_FOR_DESTINATION;
			emit fieldActivated(position);
		}
		else if (state == State::WAITING_FOR_DESTINATION) {
			destinationPosition = position;
			clear();
			emit moveIsReady();
		}
	}

signals:
	void moveIsReady();
	void fieldActivated(std::string);
};