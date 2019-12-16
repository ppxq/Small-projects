"""
Implement a webpage Crawler to crawl webpages of http://www.wikipedia.org/. 
To simplify the question, let's use url instead of the the webpage content.

Call HtmlHelper.parseUrls(url) to get all urls from a webpage of given url.
Only crawl the webpage of wikipedia.
Do not crawl the same webpage twice.
Start from the homepage of wikipedia: http://www.wikipedia.org/

"""


"""
class HtmlHelper:
    # @param (string)
    # @return (list)
    @classmethod
    def parseUrls(url):
        # Get all urls from a webpage of given url. 
"""

import threading

class Solution:
    """
    @param url(string): a url of root page
    @return (list): all urls
    """
    def crawler(self, url):
        self.queue = [url]
        self.urlSet = set([url])
        self.threads = []
        
        self.bfs()
        
        for thread in self.threads:
            thread.join()
        
        return list(self.urlSet)
    
    def bfs(self):
        while len(self.queue):
            url = self.queue.pop(0)
            sub_urls = HtmlHelper.parseUrls(url)
            for sub_url in sub_urls:
                if not "wikipedia" in sub_url:
                    continue
                if sub_url in self.urlSet:
                    continue
                self.urlSet.add(sub_url)
                self.queue.append(sub_url)
                if threading.activeCount() < 3:
                    thread = threading.Thread(target = self.bfs, args = (self, ))
                    self.threads.append(thread)
                    thread.start()