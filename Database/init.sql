CREATE TABLE cell_data (
    id SERIAL PRIMARY KEY,
    type TEXT NOT NULL,
    coordinates TEXT NOT NULL,
    timestamp BIGINT NOT NULL,
    cell_id BIGINT NOT NULL,
    signal_strength INT NOT NULL,
    tracking_area_code BIGINT NOT NULL,
    operator TEXT NOT NULL,
    rsrp INT NOT NULL,
    rsrq INT NOT NULL
);