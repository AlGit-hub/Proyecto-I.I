/* Lumi · service worker: guarda la página y las librerías para abrirla sin internet.
   El modelo de Vosk NO se guarda aquí: Vosk ya lo guarda solo en el navegador. */
const VERSION = 'lumi-v5';
const BASE = [
  './', './index.html', './manifest.webmanifest',
  './lib/vosk.js',
  './fonts/bricolage-grotesque-latin.woff2',
  './icons/icon-192.png', './icons/icon-512.png', './icons/icon-maskable-512.png', './icons/apple-touch-icon.png'
];
self.addEventListener('install', e => {
  e.waitUntil(caches.open(VERSION).then(c => Promise.all(BASE.map(u => c.add(u).catch(() => {})))).then(() => self.skipWaiting()));
});
self.addEventListener('activate', e => {
  e.waitUntil(caches.keys().then(ks => Promise.all(ks.filter(k => k !== VERSION).map(k => caches.delete(k)))).then(() => self.clients.claim()));
});
self.addEventListener('fetch', e => {
  const req = e.request;
  if (req.method !== 'GET') return;
  const url = new URL(req.url);
  if (url.origin !== location.origin) return;                       // lo de otros sitios, normal
  if (/\/(modelo-vosk)\//.test(url.pathname)) return;        // modelos: sin tocar
  const esPagina = req.mode === 'navigate' || url.pathname.endsWith('/index.html') || url.pathname.endsWith('/');
  if (esPagina) {   // primero la red (así llegan tus cambios); sin internet, la copia guardada
    e.respondWith(fetch(req).then(r => { if (r.ok) { const copia = r.clone(); caches.open(VERSION).then(c => c.put('./index.html', copia)); } return r; })
      .catch(() => caches.match('./index.html')));
    return;
  }
  e.respondWith(caches.match(req).then(h => h || fetch(req).then(r => { const copia = r.clone(); caches.open(VERSION).then(c => c.put(req, copia)); return r; })));
});
