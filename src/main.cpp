#include <iostream>
#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string
#include <chrono>
#include <thread>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive

#include "tokenizer.h"
#include "nlp/sentencevectormap.h"
std::string toUppercase(const std::string &str) {
    std::string result = "";

    for (char ch: str) {
        // Convert each character to lowercase using tolower
        result += toupper(ch);
    }

    return result;
}


std::string toLowercase(const std::string &str) {
    std::string result = "";

    for (char ch: str) {
        // Convert each character to lowercase using tolower
        result += tolower(ch);
    }

    return result;
}


int main() {

    using namespace ftxui;

    // The data:
    std::string first_name;
    std::string last_name;
    std::string password;
    std::string phoneNumber;

    const LinearGradient baseColor = LinearGradient(Color::SkyBlue1, Color::DeepSkyBlue4);
    const std::string initialSpace = "            ";
    const std::string secondSpace = "                ";
    const std::string thirdSpace = "                ";

    std::vector<Element> statements;

    std::atomic<bool> responding = false;

    Component input_first_name;

    // The basic input components:
    InputOption input_option;
    input_option.multiline = false;


    Component renderer;
    auto screen = ScreenInteractive::TerminalOutput();

    //Initialize tokenizer
    tokenizer tok;
    //Initialize sentence vector mapper
    sentencevectormap sentenceVecMap(100);


    //Insert chatbot response in chat

    std::vector<std::thread> threads;
    input_option.on_enter = [&] {
        if (responding) {
            return;
        }
        //Insert user response in chat
        statements.push_back(hbox({color(baseColor, text("YOU: ")), color(Color::BlueLight, text(first_name))}));


        //Insert chat-bot response in chat
        std::basic_string prompt = toLowercase(first_name);

        //Begin with Natural Language Processing (NLP)

        //Tokenize
        std::vector<std::string> tokens = tok.tokenize(prompt, ' ');

        //Find average vector representation of this sentence
        std::vector<double> vec = sentenceVecMap.get_vector(tokens);


        if (prompt.find("good") != std::string::npos || prompt.find("fine") != std::string::npos) {
            statements.push_back(hbox({color(baseColor, text("AI: ")),
                                       color(Color::BlueLight, text("GOOD TO HEAR!"))}));
            threads.emplace_back(
                    [&baseColor, &statements, &screen, &responding]() {
                        responding = true;
                        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                        statements.push_back(hbox({color(baseColor, text("AI: ")),
                                                   color(Color::BlueLight, text("TELL ME MORE ABOUT YOURSELF"))}));
                        screen.PostEvent(Event::Custom);
                        responding = false;
                    });

        } else if (prompt.find("bad") != std::string::npos || prompt.find("terrible") != std::string::npos) {
            statements.push_back(hbox({color(baseColor, text("AI: ")),
                                       color(Color::BlueLight, text("I AM SORRY TO HEAR THAT."))}));
            threads.emplace_back([&baseColor, &statements, &screen, &responding]() {
                responding = true;
                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                statements.push_back(hbox({color(baseColor, text("AI: ")),
                                           color(Color::BlueLight, text("WHAT IS BOTHERING YOU?"))}));
                screen.PostEvent(Event::Custom);
                responding = false;

            });
        } else if (prompt.find('i') != std::string::npos && prompt.find("my") != std::string::npos) {
            std::string response = prompt.replace(prompt.find('i'), 1, "YOU")
                    .replace(prompt.find("my"), 2, "YOUR");

            if (response.find("me") != std::string::npos) {
                response = response.replace(prompt.find("me"), 2, "YOU");
            }
            statements.push_back(hbox({color(baseColor, text("AI: ")),
                                       color(Color::BlueLight, text(toUppercase(response)))}));
        } else {
            statements.push_back(hbox({color(baseColor, text("AI: ")),
                                       color(Color::BlueLight,
                                             text("COULD YOU ELABORATE? I DID NOT UNDERSTAND YOU"))}));
        }

        //Insert new user chat box option for the next message
        InputOption new_input_option;
        new_input_option.multiline = false;
        new_input_option.on_enter = input_option.on_enter;
        first_name = "";
        input_first_name = Input(&first_name, "", new_input_option);
    };

    input_first_name = Input(&first_name, "", input_option);
    Component input_last_name = Input(&last_name, "last name");

    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "password", password_option);

    // The phone number input component:
    // We are using `CatchEvent` to filter out non-digit characters.
    Component input_phone_number = Input(&phoneNumber, "phone number");
    input_phone_number |= CatchEvent([&](Event event) {
        return event.is_character() && !std::isdigit(event.character()[0]);
    });
    input_phone_number |= CatchEvent([&](Event event) {
        return event.is_character() && phoneNumber.size() > 10;
    });

    // The component tree:
    auto component = Container::Vertical({
                                                 input_first_name,
                                                 input_last_name,
                                                 input_password,
                                                 input_phone_number,
                                         });

    // Tweak how the component tree is rendered:
    renderer = Renderer(component, [&] {
        return vbox({
                            separator(),
                            color(baseColor, text("Welcome to")),
                            //HOW ARE YOU
                            text(""),
                            color(baseColor, text(initialSpace + "HH   HH    OOO    WW    WWW    WW")),
                            color(baseColor, text(initialSpace + "HH   HH   OO OO   WW    WWW    WW")),
                            color(baseColor, text(initialSpace + "HHHHHHH  OO   OO  WW   WW WW   WW")),
                            color(baseColor, text(initialSpace + "HH   HH   OO OO    WW WW   WW WW")),
                            color(baseColor, text(initialSpace + "HH   HH    OOO      WWW     WWW")),
                            text(""),
                            color(baseColor, text(secondSpace + " AAAAA   RRRRR   EEEEEE")),
                            color(baseColor, text(secondSpace + "AA   AA RR   RR  EE")),
                            color(baseColor, text(secondSpace + "AAAAAAA RRRRRRR  EEEEEE")),
                            color(baseColor, text(secondSpace + "AA   AA RR  RR   EE")),
                            color(baseColor, text(secondSpace + "AA   AA RR   RR  EEEEEE")),
                            text(""),
                            color(baseColor, text(thirdSpace + "YY   YY    OOO    UU   UU")),
                            color(baseColor, text(thirdSpace + "YY   YY   OO OO   UU   UU")),
                            color(baseColor, text(thirdSpace + " YY YY   OO   OO  UU   UU")),
                            color(baseColor, text(thirdSpace + "  YYY      OO OO  UU   UU")),
                            color(baseColor, text(thirdSpace + "  YYY       OOO     UUU")),
                            separator(),
                            hbox({
                                         color(baseColor, text("AI: ")),
                                         color(Color::BlueLight, text("HOW ARE YOU?"))}),
                            vbox(statements),

                            hbox({
                                         color(baseColor, text("YOU: ")),
                                         input_first_name->Render()}),
                    }); //|
        //border;
    });

    screen.Loop(renderer);
    threads.clear();
    return 0;
}
