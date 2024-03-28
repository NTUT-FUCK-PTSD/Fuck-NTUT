//
// Created by adven on 2024/3/27.
//
#include "Music/Tempo.h"

Tempo::Tempo() {}

Tempo::~Tempo() {}

void Tempo::readTempoFile(const std::string &txtFilePath) {
    std::ifstream txtTempoFile(txtFilePath);
    LOG_INFO("hello");

    if (!txtTempoFile.is_open()) {
        LOG_ERROR("fuck it can not open this file");
        return;
    }

    std::string line;
    std::getline(txtTempoFile, line);

    m_tempoList = std::move(txtToVector(line, ','));

    // 关闭文件流
    txtTempoFile.close();
}

std::vector<std::size_t> Tempo::txtToVector(const std::string &line,
                                            const char splitChar) {
    std::stringstream ss(line);
    std::string item;
    std::size_t transform = 0;
    std::vector<std::size_t> elems;
    while (std::getline(ss, item, splitChar)) {
        transform = std::stoi(item);
        elems.push_back(std::move(transform));
    }

    m_tempoListLength = elems.size();
    return elems;
}

bool Tempo::canBeClick(std::size_t offset) {

    const auto tempoIndex = m_tempoIndex + m_punishTimes;

    if (m_duringTime >= m_tempoList[tempoIndex] - 500 &&
        m_duringTime <= m_tempoList[tempoIndex] + 500) {
        return true;
    }
    return false;
}

void Tempo::UpdateTime() {
    if (m_tempoList.empty()) {
        return;
    }

    if (m_duringTime >= m_tempoList[m_tempoIndex] - 500 &&
        m_duringTime <= m_tempoList[m_tempoIndex] + 500) {
        return;
    }
    else if (m_duringTime <= m_tempoList[m_tempoIndex + 1] - 500 &&
             m_duringTime <= m_tempoList[m_tempoIndex + 1] + 500) {
        return;
    }

    m_punishTimes = m_punishTimes == 0 ? m_punishTimes : m_punishTimes - 1;
    m_tempoIndex++;

    LOG_DEBUG(m_punishTimes);
}

void Tempo::Update() {
    UpdateTime();
}
