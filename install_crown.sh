#!/bin/bash

echo "Обновление списка пакетов..."
sudo apt-get update

echo "Установка необходимых зависимостей..."
sudo apt-get install -y wget curl

CROW_VERSION="1.2.0"
CROW_DEB="Crow-$CROW_VERSION-Linux.deb"

echo "Скачивание Crow версии $CROW_VERSION..."
wget https://github.com/CrowCpp/Crow/releases/download/v$CROW_VERSION/$CROW_DEB

echo "Установка Crow..."
sudo dpkg -i $CROW_DEB

if [ $? -ne 0 ]; then
    echo "Ошибка при установке Crow. Попробуем исправить зависимости..."
    sudo apt-get install -f
fi

echo "Проверка установки Crow..."
dpkg -l | grep crow

echo "Удаление загруженного файла..."
rm $CROW_DEB

echo "Установка Crow завершена!"
