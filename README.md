# Capybara API

## API Docs
The swagger file can be found here: `./swagger-docs.yaml`.

## Build & Run
Create `.env` file and fill it with values:
```bash
cp env-template .env
```

To build and start the server, run the following command:
```bash
docker-compose up
```

## Details
The port, the server will be listening on, can be specified in .env file: 
```
SERVER_PORT=<your_port>
```
