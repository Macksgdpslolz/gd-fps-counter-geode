#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <chrono>
#include <cstdio>

using namespace geode::prelude;

class FPSCounterNode : public CCNode {
public:
    CCLabelBMFont* m_label = nullptr;
    int m_frames = 0;
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
        m_label->setScale(0.4f);
        m_label->setOpacity(180);
        m_label->setAnchorPoint({0.f, 1.f});
        this->addChild(m_label);
        this->scheduleUpdate();
        return true;
    }

    void update(float) override {
        m_frames++;
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - m_last).count();
        if (elapsed >= 0.25) {
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

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto node = FPSCounterNode::create();
        node->setPosition({5.f, winSize.height - 5.f});
        node->setID("fps-counter"_spr);
        this->addChild(node, 999);
        return true;
    }
};
