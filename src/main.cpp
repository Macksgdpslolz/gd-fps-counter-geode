#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <chrono>
#include <cstdio>
#include <string>

using namespace geode::prelude;

class FPSCounterNode : public CCNode {
public:
    CCLabelBMFont* m_label = nullptr;
    int m_frames = 0;
    double m_interval = 0.25;
    std::chrono::steady_clock::time_point m_last = std::chrono::steady_clock::now();

    static FPSCounterNode* create() {
        auto ret = new FPSCounterNode();
        if (ret && ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool init() override {
        if (!CCNode::init()) return false;
        m_label = CCLabelBMFont::create("FPS: 0", "bigFont.fnt");
        this->addChild(m_label);
        this->applySettings();
        this->scheduleUpdate();
        return true;
    }

    void applySettings() {
        if (!m_label) return;

        auto mod = Mod::get();
        m_interval = mod->getSettingValue<double>("update-interval");
        if (m_interval < 0.05) m_interval = 0.05;

        float scale = static_cast<float>(mod->getSettingValue<double>("scale"));
        int opacity = static_cast<int>(mod->getSettingValue<int64_t>("opacity"));
        if (opacity < 0) opacity = 0;
        if (opacity > 255) opacity = 255;

        std::string corner = mod->getSettingValue<std::string>("corner");
        float offsetX = static_cast<float>(mod->getSettingValue<double>("offset-x"));
        float offsetY = static_cast<float>(mod->getSettingValue<double>("offset-y"));

        m_label->setScale(scale);
        m_label->setOpacity(static_cast<GLubyte>(opacity));

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        if (corner == "Top-Right") {
            m_label->setAnchorPoint({1.f, 1.f});
            this->setPosition({winSize.width - offsetX, winSize.height - offsetY});
        } else if (corner == "Bottom-Left") {
            m_label->setAnchorPoint({0.f, 0.f});
            this->setPosition({offsetX, offsetY});
        } else if (corner == "Bottom-Right") {
            m_label->setAnchorPoint({1.f, 0.f});
            this->setPosition({winSize.width - offsetX, offsetY});
        } else {
            m_label->setAnchorPoint({0.f, 1.f});
            this->setPosition({offsetX, winSize.height - offsetY});
        }
    }

    void update(float) override {
        this->applySettings();

        m_frames++;
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - m_last).count();
        if (elapsed >= m_interval) {
            int fps = static_cast<int>(m_frames / elapsed + 0.5);
            char buf[32];
            std::snprintf(buf, sizeof(buf), "FPS: %d", fps);
            m_label->setString(buf);
            m_frames = 0;
            m_last = now;
        }
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        auto node = FPSCounterNode::create();
        node->setID("fps-counter"_spr);
        this->addChild(node, 999);
        return true;
    }
};
