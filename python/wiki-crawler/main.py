import requests
import base64
import time
from requests_html import HTMLSession

session = HTMLSession()
links_to_visit = set()
links_already_visited = set()
pages = set()


class Page:
    def __init__(self, id: str, content: str, links: set, isPortal: bool):
        self.id = id
        self.content = content
        self.links = links
        self.isPortal = isPortal

    def __str__(self):
        return str(self.__class__) + '\n' + '\n'.join(('{} = {}'.format(item, self.__dict__[item]) for item in self.__dict__))

    __repr__ = __str__


def crawl(url: str, counter: int):
    if url in links_already_visited:
        print(f"{url} already crawled")
    else:
        print(f"Crawling site: {url}")

        r = session.get(url)

        if "portal" in url.lower():
            print("This is a portal site")
            left_col = r.html.find('.portal-column-left-wide', first=True)
            links_to_visit.update(left_col.absolute_links)
            links_already_visited.add(url)
            # pages.add(Page(f"P-{counter}", left_col.text, left_col.absolute_links))

        else:
            print("This is not a portal site")
            mw_content_text = r.html.find('#mw-content-text > div > p')
            text = ""
            for val in mw_content_text:
                text += val.text
                links_to_visit.update(val.absolute_links)

            links_already_visited.add(url)
            pages.add(Page(f"P-{counter}", text.replace('\n', ' ').replace('\r', ''),
                           set(), False))


def writeToFile(pagesList: list):
    f = open("wikipedia_crawls.docs", "a")
    for val in pagesList:
        f.write(f"{val.id}~{val.content}\n")
    f.close()


def startCrawl(entry: str):
    counter = 0
    crawl(entry, counter)
    while links_to_visit - links_already_visited:
        for k in links_to_visit - links_already_visited:
            crawl(k, counter)
            print(counter)
            counter += 1
            if (counter == 3):
                break
            time.sleep(2)
        if (counter == 3):
            break


if __name__ == '__main__':

    startCrawl('https://en.wikipedia.org/wiki/Tooth_enamel')

    writeToFile(list(pages))
