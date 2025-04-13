"""
To setup run:
```python
python -m venv .venv
source .venv/bin/activate
pip install fastapi 'uvicorn[standard]'
```

To run server:
```python
uvicorn main:app --port 8888
```

You can see swagger ui in http://localhost:<port>/docs
"""
import time
import random
import secrets
from enum import Enum
from datetime import datetime

from fastapi import FastAPI, APIRouter, HTTPException, Response
from pydantic import BaseModel, Field

class ConcertStatusEnum(str, Enum):
    scheduled = "scheduled"
    completed = "completed"

class TicketStatusEnum(str, Enum):
    unused = "unused"
    used = "used"

class Concert(BaseModel):
    id: int = Field(examples=[123])
    title: str = Field(examples=["Title"])
    location: str = Field(examples=["Location"])
    date: datetime = Field(examples=["2025-01-01"])
    current_participant_count: int = Field(examples=[100])
    status: ConcertStatusEnum = Field(examples=[ConcertStatusEnum.scheduled])

    @staticmethod
    def get_random():
        id = random.randint(0, 1000)
        title = f"Concert with id {id}"
        location = f"Location"
        date = datetime.now()
        current_participant_count = random.randint(0, 100)
        status = (
            ConcertStatusEnum.scheduled
            if random.uniform(0, 1) > 0.5
            else ConcertStatusEnum.completed
        )
        return Concert(
            id=id,
            title=title,
            location=location,
            date=date,
            current_participant_count=current_participant_count,
            status=status,
        )

class Ticket(BaseModel):
    id: int = Field(examples=[123])
    concert_id: int = Field(examples=[123])
    validation_token: str = Field(examples=["AABBCC123"])
    date: datetime = Field(examples=["2025-01-01"])
    status: TicketStatusEnum = Field(examples=[TicketStatusEnum.unused])

    @staticmethod
    def generate_validation_token():
        return secrets.token_hex(10)

    @staticmethod
    def get_random():
        return Ticket(
            id = random.randint(0, 10000),
            concert_id = random.randint(0, 1000),
            validation_token = Ticket.generate_validation_token(),
            date = datetime.now(),
            status = TicketStatusEnum.unused if random.uniform(0, 1) > 0.5 else TicketStatusEnum.used
        )


app = FastAPI()
concerts_router = APIRouter(prefix="/concerts", tags=["concerts"])
tickets_router = APIRouter(prefix="/tickets", tags=["tickets"])


@concerts_router.get(
    "/",
    response_model=list[Concert],
    responses={200: {"description": "Successful operation"}},
)
def get_concerts() -> list[Concert]:
    """Gets all concerts"""
    result = []
    for i in range(random.randint(0, 3)):
        result.append(Concert.get_random())
    return result


@concerts_router.post(
    "/",
    response_model=Concert,
    responses={
        200: {"description": "Successful operation"},
        400: {"description": "Invalid input"},
        422: {"description": "Validation error"},
        500: {"description": "Server error"},
    },
)
def create_concert(concert: Concert) -> Concert:
    """Creates concert"""
    r = random.uniform(0, 1)
    if r > 0.9:
        raise HTTPException(400, detail="Concert not found in request body")
    elif r > 0.8:
        raise HTTPException(422, detail="Invalid date")
    elif r > 0.7:
        raise HTTPException(500, detail="Failed to save to db")

    return concert


@concerts_router.get(
    "/{concert_id}",
    response_model=Concert,
    responses={
        200: {"description": "Successful operation"},
        404: {"description": "Concert not found"}
        },
)
def get_concert(concert_id: int) -> Concert:
    """Gets concert by id"""
    if random.uniform(0, 1) > 0.9:
        raise HTTPException(404, detail="Concert not found")

    concert = Concert.get_random()
    concert.id = concert_id
    concert.title = "Test"
    return concert

@concerts_router.put(
    "/{concert_id}",
    response_model=Concert,
    responses={
        200: {"description": "Successful operation"},
        400: {"description": "Invalid input"},
        404: {"description": "Concert not found"},
        422: {"description": "Validation error"},
        500: {"description": "Server error"},
        },
)
def update_concert(concert_id: int, concert: Concert) -> Concert:
    """Updates concert"""
    r = random.uniform(0, 1)
    if r > 0.9:
        raise HTTPException(400, detail="Concert not found in body")
    elif r > 0.8:
        raise HTTPException(404, detail="Concert not found")
    elif r > 0.7:
        raise HTTPException(422, detail="Invalid date")
    elif r > 0.5:
        raise HTTPException(500, detail="Failed to update db")

    return concert

@concerts_router.delete(
    "/{concert_id}",
    response_model=Concert,
    responses={
        200: {"description": "Successful operation"},
        404: {"description": "Concert not found"},
        500: {"description": "Server error"},
        },
)
def delete_concert(concert_id: int) -> Concert:
    """Deletes concert by id"""
    r = random.uniform(0, 1)
    if r > 0.8:
        raise HTTPException(404, detail="Concert not found")
    elif r > 0.5:
        raise HTTPException(500, detail="Failed to update db")

    return Response()

@concerts_router.patch(
    "/{concert_id}/validate_tickets",
    response_model=None,
    responses={
        200: {"description": "Successful operation"},
        400: {"description": "Invalid input"},
        404: {"description": "Concert not found"},
        422: {"description": "Validation error"},
        500: {"description": "Server error"},
        },
)
def validate_concert_ticket(concert_id: int, ticket: Ticket) -> None:
    """Validates concert ticket and updates concert participants count"""
    r = random.uniform(0, 1)
    if r > 0.9:
        raise HTTPException(400, detail="Ticket not found in request body")
    elif r > 0.8:
        raise HTTPException(404, detail="Concert not found")
    elif r > 0.7:
        raise HTTPException(422, detail="Ticket is not valid")
    elif r > 0.6:
        raise HTTPException(500, detail="Failed to update db")

    return Response()

@tickets_router.get(
    "/",
    response_model=list[Ticket],
    responses={
        200: {"description": "Successful operation"},
        },
)
def get_tickets() -> list[Ticket]:
    """Gets tickets"""
    result = []

    for i in range(random.randint(0, 5)):
        result.append(Ticket.get_random())

    return result

@tickets_router.post(
    "/",
    response_model=Ticket,
    responses={
        200: {"description": "Successful operation"},
        400: {"description": "Invalid input"},
        422: {"description": "Validation error"},
        500: {"description": "Server error"},
    },
)
def create_ticket(ticket: Ticket) -> Ticket:
    """Creates ticket"""
    r = random.uniform(0, 1)
    if r > 0.9:
        raise HTTPException(400, detail="Ticket not found in request body")
    elif r > 0.8:
        raise HTTPException(422, detail="Invalid date")
    elif r > 0.7:
        raise HTTPException(500, detail="Failed to save to db")

    return ticket


@tickets_router.get(
    "/{ticket_id}",
    response_model=Ticket,
    responses={
        200: {"description": "Successful operation"},
        404: {"description": "Ticket not found"}
        },
)
def get_ticket(ticket_id: int) -> Concert:
    """Gets ticket by id"""
    if random.uniform(0, 1) > 0.9:
        raise HTTPException(404, detail="Ticket not found")

    ticket = Ticket.get_random()
    ticket.id = ticket_id
    return ticket

@tickets_router.put(
    "/{ticket_id}",
    response_model=Ticket,
    responses={
        200: {"description": "Successful operation"},
        400: {"description": "Invalid input"},
        404: {"description": "Ticket not found"},
        422: {"description": "Validation error"},
        500: {"description": "Server error"},
        },
)
def update_ticket(ticket_id: int, ticket: Ticket) -> Ticket:
    """Updates ticket"""
    r = random.uniform(0, 1)
    if r > 0.9:
        raise HTTPException(400, detail="Ticket not found in body")
    elif r > 0.8:
        raise HTTPException(404, detail="Ticket not found")
    elif r > 0.7:
        raise HTTPException(422, detail="Invalid date")
    elif r > 0.5:
        raise HTTPException(500, detail="Failed to update db")

    return ticket

@tickets_router.delete(
    "/{ticket_id}",
    response_model=Concert,
    responses={
        200: {"description": "Successful operation"},
        404: {"description": "Ticket not found"},
        500: {"description": "Server error"},
        },
)
def delete_ticket(ticket_id: int) -> Ticket:
    """Deletes ticket by id"""
    r = random.uniform(0, 1)
    if r > 0.8:
        raise HTTPException(404, detail="Ticket not found")
    elif r > 0.5:
        raise HTTPException(500, detail="Failed to update db")

    return Response()

@tickets_router.patch(
    "/{ticket_id}",
    response_model=Ticket,
    responses={
        404: {"description": "Ticket not found"},
        408: {"description": "Request timeout"},
        429: {"description": "Too many requests"},
        500: {"description": "Server error"},
        },
)
def generate_ticket_token(ticket_id: int) -> Ticket:
    """Generates new validation token for ticket"""
    r = random.uniform(0, 1)
    if r > 0.8:
        raise HTTPException(404, detail="Ticket not found")
    elif r > 0.7:
        raise HTTPException(408, detail="Request timeout")
    elif r > 0.6:
        raise HTTPException(429, detail="Too many requests")
    elif r > 0.5:
        raise HTTPException(500, detail="Failed to update db")

    time.sleep(random.randint(1, 5))
    ticket = Ticket.get_random()
    ticket.id = ticket_id
    return ticket

app.include_router(concerts_router)
app.include_router(tickets_router)
