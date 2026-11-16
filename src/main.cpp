#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <chrono>
#include <cmath>

using namespace geode::prelude;

class $modify(FPSPlayLayer, PlayLayer) {
    struct Fields {
        Ref<CCLabelBMFont> m_fpsLabel;
        std::chrono::steady_clock::time_point m_lastTime =
            std::chrono::steady_clock::now();
        int m_frameCount = 0;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) {
            return false;
        }

        auto label = CCLabelBMFont::create("FPS: 0", "bigFont.fnt");
        if (label) {
            label->setScale(0.4f);
            label->setOpacity(180);
            label->setAnchorPoint({0.f, 1.f});
            label->setPosition({5.f, CCDirector::sharedDirector()->getWinSize().height - 5.f});
            label->setID("fps-label"_spr);
            this->addChild(label, 999);

            m_fields->m_fpsLabel = label;
            m_fields->m_lastTime = std::chrono::steady_clock::now();
            m_fields->m_frameCount = 0;
        }

        return true;
    }

    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        m_fields->m_frameCount++;
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - m_fields->m_lastTime).count();

        if (elapsed >= 0.25) {
            int fps = static_cast<int>(std::lround(static_cast<double>(m_fields->m_frameCount) / elapsed));
            if (m_fields->m_fpsLabel) {
                m_fields->m_fpsLabel->setString(fmt::format("FPS: {}", fps).c_str());
            }
            m_fields->m_frameCount = 0;
            m_fields->m_lastTime = now;
        }
    }
};
