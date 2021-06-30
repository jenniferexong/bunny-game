#include "GuiComponent.h"

#include "GuiEvent.h"
#include "GuiTexture.h"
#include "GuiText.h"
#include "GuiBound.h"
#include "TextMaster.h"
#include "../util/Log.h" 
#include "../util/JsonParser.h" 

using namespace glm;
using namespace std;

GuiComponent::~GuiComponent()
{
	for (const auto& c: components_)
		delete c;

	delete hover_event_;
	delete click_event_;
	delete off_hover_event_;
}

/**
 * Load a GuiComponent from a json file
 */
GuiComponent::GuiComponent(const string& file_name): GuiComponent()
{
    JsonValue data = JsonParser::parse(file_name);

    // json object
    if (has<Object>(data)) {
        Object obj = get<Object>(data);
        addChildComponent(obj);
    }
    else if (has<Array>(data)) {
        Array arr = get<Array>(data);
        for (const auto& v: arr.getValues()) {
            Object obj = get<Object>(v);
            addChildComponent(obj);
        }
    }
}

void GuiComponent::addChildComponent(const Object& component_obj)
{
    const static string bound = "bound";
    component_obj.require({bound});

    // must have "bound"
    Object bound_obj = getObject(component_obj, bound);
    GuiBound child_bound = makeBound(bound_obj);
    shared_ptr<GuiBound> new_bound = 
        make_shared<GuiBound>(child_bound, *bound_);

    GuiComponent* new_component = new GuiComponent(
        this, new_bound);

    // optional settings
    const static string id = "id";
    const static string components = "components";
    const static string gui = "gui";
    const static string text = "text";

    // id
    if (component_obj.hasKey(id))
        new_component->setId(get<String>(component_obj.get(id)));

    // adding components
    if (component_obj.hasKey(components)) {
        Array array = get<Array>(component_obj.get(components));
        for (const JsonValue& c: array.getValues())
            new_component->addChildComponent(get<Object>(c));
    }

    // gui
    if (component_obj.hasKey(gui)) {
        Object gui_obj = getObject(component_obj, gui);
        new_component->setGuiTexture(gui_obj);
    }

    // text
    if (component_obj.hasKey(text)) {
        new_component->setText(
            getObject(component_obj, text), *new_component->bound_);
    }

    components_.push_back(new_component);
}

GuiBound GuiComponent::makeBound(const Object& bound_obj)
{
    const static string pos = "position";
    const static string sc = "scale";

    const static string pixel_pos = "pixel-position";
    const static string s = "size";

    vector<float> nums;
    // "position" and "scale"
    if (bound_obj.hasKey(pos)) {
        bound_obj.require({pos, sc});
        nums = getNumbers(bound_obj, pos);
        vec2 position = vec2(nums[0], nums[1]);
        nums = getNumbers(bound_obj, sc);
        vec2 scale = vec2(nums[0], nums[1]);
        return { position, scale };
    }
    // "pixel-position" and "size"
    else if(bound_obj.hasKey(pixel_pos)) {
        bound_obj.require({pixel_pos, s});
        nums = getNumbers(bound_obj, pixel_pos);
        ivec2 pixel_position = ivec2(nums[0], nums[1]);
        nums = getNumbers(bound_obj, s);
        ivec2 size = ivec2(nums[0], nums[1]);
        return { pixel_position, size, *bound_};
    }
    else {
        Error::exit(
            "GuiComponent::makeBound(), missing position or pixel-position");
        return {};
    }
}

void GuiComponent::setGuiTexture(const Object& gui_obj)
{
    const static string t = "texture";
    const static string c = "color";

    // optional
    const static string h = "hover-color";

    gui_ = make_shared<GuiTexture>();
    gui_->setBounds(bound_);
    if (gui_obj.hasKey(t)) {
        int texture = GuiTexture::getTexture(get<String>(gui_obj.get(t)));
        gui_->setTexture(texture);
    }
    else if (gui_obj.hasKey(c)) {
        auto nums = getNumbers(gui_obj, c);
        vec4 color = vec4(nums[0], nums[1], nums[2], nums[3]);
        gui_->setColor(color);
        if (gui_obj.hasKey(h)) {
            nums = getNumbers(gui_obj, h);
            color = vec4(nums[0], nums[1], nums[2], nums[3]);
            setHoverColor(color);
        }
    }
    else
        Error::exit("missing \"color\" or \"texture\" for GuiTexture (json)");

    // optional settings
    const static string flip = "flip";
    const static string transparency = "transparency";

    if (gui_obj.hasKey(flip)) {
        bool f = get<Bool>(gui_obj.get(flip));
        if (f)
            gui_->setFlipVertically();
    }
    if (gui_obj.hasKey(transparency)) {
        bool t = get<Bool>(gui_obj.get(transparency));
        gui_->setTransparency(t);
    }
}

void GuiComponent::setText(const Object& text_obj, const GuiBound& bound)
{
    const static string content = "content";
    const static string size = "size";
    const static string font = "font";
    const static string position = "position";
    const static string width = "width";
    const static string centered = "centered";
    text_obj.require({content, size, font, position, width, centered});
    auto nums = getNumbers(text_obj, position);

    // construct GuiText
    string text_content = get<String>(text_obj.get(content));
    float font_size = get<Number>(text_obj.get(size));
    string font_name = get<String>(text_obj.get(font));
    vec2 text_position = vec2(nums[0], nums[1]);
    float max_line_width = get<Number>(text_obj.get(width));
    bool center_text = get<Bool>(text_obj.get(centered));

    text_ = make_shared<GuiText>(
        text_content, font_size, 
        font_name, text_position, 
        max_line_width, center_text,
        bound
    );

    // optional settings
    const static string color = "color";

    if (text_obj.hasKey("color")) {
        nums = getNumbers(text_obj, color);
        vec3 text_color = vec3(nums[0], nums[1], nums[2]);
        text_->setColor(text_color);
    }
}

/**
 * Creates a base GuiComponent which is the size of the 
 * entire window, has no GuiTexture or GuiText
 */
GuiComponent::GuiComponent()
{
	parent_ = nullptr;
    bound_ = make_shared<GuiBound>(vec2(0.5), vec2(1));

	hover_event_ = new GuiEvent();
	off_hover_event_ = new GuiEvent();
	click_event_ = new GuiEvent();
}

GuiComponent::GuiComponent(
    GuiComponent* parent,
    shared_ptr<GuiBound> bound
):
    parent_(parent), bound_(bound)
{
	hover_event_ = new GuiEvent();
	off_hover_event_ = new GuiEvent();
	click_event_ = new GuiEvent();
}

GuiComponent* GuiComponent::getComponent(const string& id)
{
    GuiComponent* found = find(id);
    if (found == nullptr)
        Error::exit("GuiComponent: " + id + ", not found");

    return found;
}

GuiComponent* GuiComponent::find(const string& id)
{
	if (id_ == id)
		return this;

	for (auto& c: components_) {
		GuiComponent* found = c->find(id);
		if (found)
            return found;
	}
	return nullptr;
}

// TODO
void GuiComponent::onHover(vec2 mouse_pos) 
{
	if (!bound_->containsPoint(mouse_pos))
		off_hover_event_->excecute();
	else 
		hover_event_->excecute();

	for (const auto& c: components_)
		c->onHover(mouse_pos);
}

/**
 * Adds GuiTexture and GuiText to rendering batch
 */
void GuiComponent::addToBatch(
	vector<weak_ptr<GuiTexture>>& guis,
	TextMaster& text_master)
{
	if (gui_ != nullptr)
		guis.push_back(gui_);
	if (text_ != nullptr)
		text_master.addText(text_);

	for (const auto& c: components_)
		c->addToBatch(guis, text_master);
}

void GuiComponent::onClick(vec2 mouse_pos) 
{
	if (!bound_->containsPoint(mouse_pos))
		return;

	click_event_->excecute();

	for (const auto& c: components_)
		c->onClick(mouse_pos);
}

void GuiComponent::setHoverColor(vec4 color) 
{
	setHoverEvent(new ChangeColor(color, gui_));
	setOffHoverEvent(new ChangeColor(gui_->getColor(), gui_));
}

void GuiComponent::setHoverEvent(GuiEvent* event) 
{
	hover_event_ = event;
}

void GuiComponent::setOffHoverEvent(GuiEvent* event) 
{
	off_hover_event_ = event;
}

void GuiComponent::setClickEvent(GuiEvent* event) 
{
	click_event_ = event;
}
