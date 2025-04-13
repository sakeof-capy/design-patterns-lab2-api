main-api:
	docker compose up api

build-main-api:
	docker compose up api --build

integration:
	docker compose up integration external_api

build-integration:
	docker compose up integration external_api

stop:
	docker compose down -v
