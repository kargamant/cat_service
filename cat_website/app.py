from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi.templating import Jinja2Templates
import ipaddress

app = FastAPI()

app.mount("/static", StaticFiles(directory="static"), name="static")

templates = Jinja2Templates(directory=".")


def is_internal_ip(ip: str) -> bool:
    try:
        return ipaddress.ip_address(ip).is_private
    except ValueError:
        return False


def form_rating(cat_log: list):
    cat_log = list(map(lambda x: x.split(' '), cat_log))
    cat_log = list(map(lambda x: (x[0], int(x[1][:-1:])), cat_log))
    rating = {}
    for pair in cat_log:
        if pair[0] in rating:
            rating[pair[0]] += pair[1]
        else:
            rating[pair[0]] = 0

    rating_str = sorted(rating.items(), key=lambda x: x[1], reverse=True)
    rating_str = ''.join(list(map(lambda x: x[0] + ' ' + str(x[1]) + '\n', rating_str)))
    return rating_str


@app.get("/cat_dashboard", response_class=HTMLResponse)
async def dashboard(request: Request):
    client_ip = request.client.host
    internal = is_internal_ip(client_ip)

    if internal:
        with open('internal.html', 'r') as file:
            html = ''.join(file.readlines())
    else:
        with open('external.html', 'r') as file:
            html = ''.join(file.readlines())

    with open('cat_log.txt', 'r') as file:
        cat_log = file.readlines()

    rating = form_rating(cat_log)

    return templates.TemplateResponse(request=request, name="internal.html" if internal else "external.html", context={
                "contents": rating.__str__()
            }
    )
