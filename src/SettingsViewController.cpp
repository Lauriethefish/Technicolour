#include "SettingsViewController.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"
using namespace QuestUI;

#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
using namespace UnityEngine::UI;
#include "UnityEngine/RectOffset.hpp"

#include "HMUI/Touchable.hpp"
#include "UnityEngine/ParticleSystem.hpp"

#include <iomanip>
#include <sstream>

using namespace Technicolour;
DEFINE_TYPE(Technicolour, SettingsViewController);

#define CREATE_DROPDOWN(parentTransform, configProperty, values, parser, saver, text, hoverHint) {\
    HMUI::SimpleTextDropdown* dropDown = BeatSaberUI::CreateDropdown(parentTransform, \
    text, \
    saver(configProperty), \
    values, \
    [](std::string_view newValue) { \
        configProperty = parser(newValue); \
        saveConfig(); \
    } \
); BeatSaberUI::AddHoverHint(dropDown->get_gameObject(), hoverHint); } \

#define CREATE_STYLE_DROPDOWN(parentTransform, configProperty, text, hoverHint) CREATE_DROPDOWN(parentTransform, configProperty, TechnicolourConfig::getPossibleStyleValues(), TechnicolourConfig::loadStyle, TechnicolourConfig::saveStyle, text, hoverHint);

std::string floatToString(float x) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << x;
    return stream.str();
}

void openStylesModal(UnityEngine::RectTransform* parentTransform) {
    HMUI::ModalView* stylesModal = BeatSaberUI::CreateModal(parentTransform, {75.0f, 70.0f}, {0.0f, 15.0f}, [](HMUI::ModalView* modalView){
        UnityEngine::GameObject::Destroy(modalView->get_gameObject());
    });

    VerticalLayoutGroup* stylesLayout = BeatSaberUI::CreateVerticalLayoutGroup(stylesModal->get_transform());
    stylesLayout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 3, 3));
    stylesLayout->GetComponent<LayoutElement*>()->set_preferredWidth(70.0f);

    UnityEngine::Transform* stylesTransform = stylesLayout->get_rectTransform();

    TMPro::TextMeshProUGUI* titleText = BeatSaberUI::CreateText(stylesTransform, "Technicolour Styles");
    titleText->set_alignment(TMPro::TextAlignmentOptions::Center);
    titleText->set_fontSize(6.0f);
    titleText->set_margin(UnityEngine::Vector4 {0.0f, 1.0f, 0.0f, 0.0f});

    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().lightsStyle, "Rainbow Lights", "Technicolor style of the lights.");
    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().wallsStyle, "Rainbow Walls", "Technicolour style of the walls.");
    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().bombsStyle, "Rainbow Bombs", "Technicolor style of the bombs.");
    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().blocksStyle, "Rainbow Notes", "Technicolor style of the notes.");
    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().sabersStyle, "Rainbow Sabers", "Technicolor style of the sabers.");
    BeatSaberUI::CreateText(stylesTransform, "");
    BeatSaberUI::CreateText(stylesTransform, "");
    stylesModal->Show(true, false, nullptr);
}

void SettingsViewController::DidActivate(bool firstActivation) {
    if(!firstActivation) {
        return;
    }

    VerticalLayoutGroup* mainLayout = BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
    UnityEngine::Transform* mainTransform = mainLayout->get_transform();
    UnityEngine::Vector3 position = mainTransform->get_position();
    position.x += 1.2f;
    mainTransform->set_position(position);

    this->get_gameObject()->AddComponent<HMUI::Touchable*>();

    mainLayout->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
    mainLayout->set_childControlHeight(true);
    mainLayout->set_childForceExpandHeight(false);
    mainLayout->GetComponent<LayoutElement*>()->set_preferredWidth(65.0f);
    mainLayout->GetComponent<LayoutElement*>()->set_preferredHeight(30.0f);
    mainLayout->set_padding(UnityEngine::RectOffset::New_ctor(3, 3, 3, 3));
    //mainLayout->set_spacing(0.5f);

    Toggle* mainToggle = BeatSaberUI::CreateToggle(mainLayout->get_rectTransform(), "Enable Technicolour", getConfig().getEnabled(), [](bool newValue){
        getConfig().setEnabled(newValue);
        saveConfig();
    });
    BeatSaberUI::AddHoverHint(mainToggle->get_gameObject(), "Global toggle, enables/disables the entire mod");

    Toggle* desyncToggle = BeatSaberUI::CreateToggle(mainLayout->get_rectTransform(), "Desync Left/Right", getConfig().desync, [](bool newValue){
        getConfig().desync = newValue;
        saveConfig();
    });
    BeatSaberUI::AddHoverHint(desyncToggle->get_gameObject(), "If true, technicolor styles that shift between colors (gradient mode) will desync left/right colors.");

    Toggle* bgToggle = BeatSaberUI::CreateToggle(mainLayout->get_rectTransform(), "Disable gradient background", getConfig().disableGradientBackground, [](bool newValue) {
        getConfig().disableGradientBackground = newValue;
        saveConfig();
    });
    BeatSaberUI::AddHoverHint(bgToggle->get_gameObject(), "Disables the incredibly ugly gradient background.");


    std::vector<std::string> choices;
    for(float x = 0.05f; x <= 1.02f; x += 0.05f) {
        choices.push_back(floatToString(x));
    }

    HMUI::SimpleTextDropdown* frequencySlider = BeatSaberUI::CreateDropdown(mainLayout->get_rectTransform(), to_utf16("Lights Frequency"), floatToString(getConfig().lightsFrequency), choices, [](std::string_view newValue) {
        getConfig().lightsFrequency = std::stof(std::string(newValue));
        saveConfig();
    });
    BeatSaberUI::AddHoverHint(frequencySlider->get_gameObject(), "The higher the frequency, the more color changes. This also controls how quickly the Gradient style shifts.  10% is default.");

    CREATE_DROPDOWN(mainLayout->get_rectTransform(), getConfig().lightsGrouping, TechnicolourConfig::getPossibleGroupingValues(), TechnicolourConfig::loadGrouping, TechnicolourConfig::saveLightsGrouping, "Lights Grouping", "Set to isolated for more colours, standard for better performance");

    VerticalLayoutGroup* buttonLayout = BeatSaberUI::CreateVerticalLayoutGroup(mainLayout->get_rectTransform());
    buttonLayout->GetComponent<LayoutElement*>()->set_preferredHeight(5.0f);
    Button* button = BeatSaberUI::CreateUIButton(buttonLayout->get_rectTransform(), "Open Technicolour Styles!", "OkButton", [mainLayout](){
        openStylesModal(mainLayout->get_rectTransform());
    });

    for(int i = 0; i < 8; i++) {
        TMPro::TextMeshProUGUI* bodgeText = BeatSaberUI::CreateText(mainLayout->get_transform(), " ");
        UnityEngine::Transform* bodgeTransform = bodgeText->get_transform();
        position = bodgeTransform->get_position();
        position.x += 1.2f;
        bodgeTransform->set_position(position);
    }
}
