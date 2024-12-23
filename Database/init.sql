CREATE TABLE cell_data (
    id SERIAL PRIMARY KEY,
    type TEXT NOT NULL,
    latitude DOUBLE PRECISION NOT NULL,
    longitude DOUBLE PRECISION NOT NULL,
    timestamp BIGINT NOT NULL,
    cell_id BIGINT NOT NULL,
    signal_strength INT NOT NULL,
    tracking_area_code BIGINT NOT NULL,
    operator TEXT NOT NULL,
    rsrp INT NOT NULL,
    rsrq INT NOT NULL
);

CREATE TABLE base_station_coordinates (
    id SERIAL PRIMARY KEY,
    cell_id BIGINT NOT NULL,
    calculated_latitude DOUBLE PRECISION NOT NULL,
    calculated_longitude DOUBLE PRECISION NOT NULL
);