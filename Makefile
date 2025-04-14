DC = docker compose
FLAGS ?=

main-api:
	$(DC) up api $(FLAGS)

integration:
	$(DC) up integration external_api $(FLAGS)

integration-logs:
	$(DC) up -d integration external_api $(FLAGS)
	$(DC) logs -f integration --tail=0

stop:
	$(DC) down -v
