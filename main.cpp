#include <iostream>
#include <memory>  // for allocator, __shared_ptr_access
#include <string>  // for char_traits, operator+, string, basic_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"  // for Ref


int main() {
    using namespace ftxui;

    // The data:
    std::string first_name;
    std::string last_name;
    std::string password;
    std::string phoneNumber;

    // The basic input components:
    InputOption password_option;
    password_option.password = true;

    Component input_first_name = Input(&first_name, "", password_option);
    Component input_last_name = Input(&last_name, "last name");
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
    const LinearGradient baseColor = LinearGradient(Color::SkyBlue1, Color::DeepSkyBlue4);
    const std::string initialSpace = "            ";
    const std::string secondSpace = "                ";
    const std::string thirdSpace = "                ";
    auto renderer = Renderer(component, [&] {
        return vbox({
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
                                         color(baseColor, text("Them: ")),
                                         color(Color::BlueLight, text("HOW ARE YOU?"))}),



                            hbox({
                                    color(baseColor, text("YOU: ")),
                                    input_first_name->Render()}),
                            /*hbox(text(" Last name  : "), input_last_name->Render()),
                            hbox(text(" Password   : "), input_password->Render()),
                            hbox(text(" Phone num  : "), input_phone_number->Render()),
                            separator(),
                            text("Hello " + first_name + " " + last_name),
                            text("Your password is " + password),
                            text("Your phone number is " + phoneNumber),*/
                    }); //|
        //border;
    });

    auto screen = ScreenInteractive::TerminalOutput();
    screen.Loop(renderer);
    return 0;
}
