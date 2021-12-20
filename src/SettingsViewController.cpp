#include "SettingsViewController.hpp"
#include "main.hpp"

#include "questui/shared/BeatSaberUI.hpp"
using namespace QuestUI;

#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
using namespace UnityEngine::UI;
#include "UnityEngine/MonoBehaviour.hpp"

#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "HMUI/Touchable.hpp"

using namespace Technicolour;
DEFINE_TYPE(Technicolour, SettingsViewController);

#define CREATE_DROPDOWN(parentTransform, configProperty, values, parser, saver, text, hoverHint) {\
    HMUI::SimpleTextDropdown* dropDown = BeatSaberUI::CreateDropdown(mainLayout->get_transform(), \
    text, \
    saver(configProperty), \
    values, \
    [](std::string_view newValue) { \
        configProperty = parser(newValue); \
    } \
); BeatSaberUI::AddHoverHint(dropDown->get_gameObject(), hoverHint); } \


#define CREATE_STYLE_DROPDOWN(parentTransform, configProperty, text, hoverHint) CREATE_DROPDOWN(parentTransform, configProperty, TechnicolourConfig::getPossibleStyleValues(), TechnicolourConfig::loadStyle, TechnicolourConfig::saveStyle, text, hoverHint);

void SettingsViewController::DidActivate(bool firstActivation) {
    if(!firstActivation) {
        return;
    }

    VerticalLayoutGroup* mainLayout = BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
    UnityEngine::Transform* mainTransform = mainLayout->get_transform();
    UnityEngine::Vector3 position = mainTransform->get_position();
    position.y += 0.7f;
    mainTransform->set_position(position);

    mainLayout->set_childAlignment(UnityEngine::TextAnchor::UpperCenter);
    mainLayout->set_childControlHeight(true);
    mainLayout->set_childForceExpandHeight(false);
    mainLayout->set_spacing(7.0f);
    mainLayout->GetComponent<LayoutElement*>()->set_preferredWidth(65.0f);

    Toggle* mainToggle = BeatSaberUI::CreateToggle(mainLayout->get_rectTransform(), "Enable Technicolour", getConfig().getEnabled(), [](bool newValue){
        getConfig().setEnabled(newValue);
    });
    BeatSaberUI::AddHoverHint(mainToggle->get_gameObject(), "Global toggle, enables/disables the entire mod");

    Toggle* desyncToggle = BeatSaberUI::CreateToggle(mainLayout->get_rectTransform(), "Desync Left/Right", getConfig().desync, [](bool newValue){
        getConfig().desync = newValue;
    });
    BeatSaberUI::AddHoverHint(desyncToggle->get_gameObject(), "If true, technicolor styles that shift between colors (Gradient) will desync left/right colors.");

    CREATE_DROPDOWN(mainLayout->get_rectTransform(), getConfig().lightsGrouping, TechnicolourConfig::getPossibleGroupingValues(), TechnicolourConfig::loadGrouping, TechnicolourConfig::saveLightsGrouping, "Lights Grouping", "Set to isolated for more colours, standard for better performance");

    VerticalLayoutGroup* stylesLayout = BeatSaberUI::CreateVerticalLayoutGroup(mainLayout->get_rectTransform());
    stylesLayout->set_spacing(6.0f);
    UnityEngine::Transform* stylesTransform = stylesLayout->get_rectTransform();

    TMPro::TextMeshProUGUI* titleText = BeatSaberUI::CreateText(stylesTransform, "Technicolour Styles");
    titleText->set_alignment(TMPro::TextAlignmentOptions::Center);
    titleText->set_fontSize(6.0f);

    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().lightsStyle, "Technicolour Lights", "Technicolor style of the lights.");
    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().wallsStyle, "Technicolor Walls", "Technicolour style of the walls.");
    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().bombsStyle, "Technicolour Bombs", "Technicolor style of the bombs.");
    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().blocksStyle, "Technicolour Notes", "Technicolor style of the notes.");
    CREATE_STYLE_DROPDOWN(stylesTransform, getConfig().sabersStyle, "Technicolour Sabers", "Technicolor style of the sabers.");
    // TODO: gradient bg
}
