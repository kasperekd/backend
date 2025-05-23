# Backend for Cell Tower Data Processing

Этот проект представляет собой backend-приложение для обработки и визуализации данных сотовых вышек, получаемых от Android-приложения [VisualProg](https://github.com/kasperekd/VisualProg).

## Описание

Система предназначена для:
- Приема данных от Android-приложения о сотовых вышках (GSM, LTE, WCDMA)
- Расчета координат базовых станций на основе полученных данных
- Визуализации расположения базовых станций на интерактивной карте

### Источник

Данные поступают от Android-приложения, которое собирает следующую информацию:
- Координаты устройства (через FusedLocationProviderClient)
- Информацию о сотовых базовых станциях (через TelephonyManager):
  - Тип сети
  - Идентификатор ячейки
  - Уровень сигнала
  - Код зоны местоположения (LAC/TAC)
  - Оператор связи

## Структура проекта

Проект состоит из трех основных модулей:

### 1. Receive Server
- Принимает входящие данные от Android-приложения
- Обрабатывает и валидирует полученные данные
- Сохраняет данные в PostgreSQL базу данных

### 2. Calculation Module
- Производит расчеты координат базовых станций на основе собранных данных
- Использует алгоритмы трилатерации для определения местоположения
- Периодически обновляет расчетные данные в базе

### 3. Web Server
- Предоставляет REST API для доступа к данным
- Отдает статические файлы веб-интерфейса
- Обеспечивает визуализацию данных на интерактивной карте

## Технологии

- C++ для backend-компонентов
- Crow framework для веб-серверов
- PostgreSQL для хранения данных
- Docker для контейнеризации
- Leaflet.js для отображения карты

## Установка и запуск

1. Клонировать репозиторий:
```bash
git clone https://github.com/kasperekd/backend.git
```

2. Запустить с помощью Docker Compose:
```bash
./start.sh
```

После запуска будут доступны следующие сервисы:
- Web Server: http://localhost:8080
- Receive Server: http://localhost:9000
- PostgreSQL: localhost:5432

## API Endpoints

### Web Server (порт 8080)
- `GET /api/base-stations/{cell_id}` - получение координат конкретной базовой станции
- `GET /api/base-stations/multiple` - получение координат всех базовых станций

### Receive Server (порт 9000)
- `POST /api/cellinfo` - endpoint для приема данных от Android-приложения
- `GET /api/health` - проверка работоспособности

## Веб-интерфейс

Веб-интерфейс доступен по адресу http://localhost:8080 и предоставляет следующие возможности:
- Интерактивная карта с отображением базовых станций
- Поиск базовой станции по Cell ID
- Возможность отображения всех базовых станций одновременно
