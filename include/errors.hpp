#include <exception>

class GameServerError : public std::exception {
public:
    enum Code {
        OK = 0,
        CHARACTER_ALREADY_EXISTS, /* 1 */
        SKIN_DOES_NOT_EXIST,     /* 2 */

        COUNT
    };

    virtual Code code() const = 0;

};

class CharacterAlreadyExists : public GameServerError {
public:
    virtual Code code() const {
        return GameServerError::Code::CHARACTER_ALREADY_EXISTS;
    }

    virtual const char* what() const noexcept override {
        return "the character already exists.";
    }
};


class SkinDoesNotExist : public GameServerError {
public:
    virtual Code code() const {
        return GameServerError::Code::SKIN_DOES_NOT_EXIST;
    }
    virtual const char* what() const noexcept override {
        return "the skin does not exist.";
    }
};
