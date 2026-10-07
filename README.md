# Strategy Game Beta ⚔️

Bu proje, C++ ile geliştirilmiş veri odaklı bir strateji oyunu betasıdır. Oyun içerisindeki kahramanlar, yaratıklar, birim türleri ve araştırma sistemleri JSON dosyaları üzerinden dinamik olarak yönetilmektedir. 

## 📂 Proje Yapısı ve Dosyalar

Proje, verileri dışarıdan okuyarak oyun mantığını işleten esnek bir yapıya sahiptir:

*   **`main.cpp`**: Oyunun ana kaynak kodu ve temel motor döngüsü.
*   **`.json` Dosyaları**:
    *   `heroes.json`: Kahramanların (heroes) özelliklerini ve statlarını içerir.
    *   `creatures.json`: Oyundaki yaratıkların verilerini barındırır.
    *   `unit_types.json`: Farklı askeri/stratejik birim türlerinin tanımları.
    *   `research.json`: Oyundaki araştırma (teknoloji/yetenek) ağacı verileri.
    *   `data.json`: Oyunun genel ayarları veya kayıt (save) verileri.
*   **`savas_sim.txt`**: Savaş simülasyonunun sonuçlarını veya loglarını barındıran metin dosyası.
*   **`main.dev` & `Makefile.win`**: Dev-C++ IDE'si proje dosyaları ve derleme (build) talimatları.

## 🚀 Kurulum ve Çalıştırma

Proje Windows ortamında (Dev-C++ veya MinGW kullanılarak) derlenmek üzere hazırlanmıştır.

### Hazır Çalıştırılabilir Dosya (Windows)
Eğer doğrudan oyunu denemek isterseniz, proje dizininde bulunan derlenmiş `main.exe` dosyasını çalıştırabilirsiniz. (JSON dosyalarının `main.exe` ile aynı dizinde bulunduğundan emin olun).

### Kaynaktan Derleme
1. [Dev-C++](https://sourceforge.net/projects/orwelldevcpp/) gibi bir IDE veya MinGW derleyicisi kurun.
2. Proje dizinindeki `main.dev` dosyasını Dev-C++ ile açın.
3. Klavyeden `F11` tuşuna basarak (Derle ve Çalıştır / Compile & Run) projeyi derleyin.
*(Alternatif olarak komut satırından `make -f Makefile.win` komutu ile derleyebilirsiniz.)*

## 🛠️ Geliştirme ve Modlama

Oyunun verileri JSON formatında tutulduğu için kodlama bilmeden de oyunu "modlayabilirsiniz":
*   Yeni bir kahraman eklemek için `heroes.json` dosyasını düzenleyin.
*   Birimlerin can/hasar dengelerini ayarlamak için `unit_types.json` ve `creatures.json` dosyalarındaki değerleri değiştirin.
*   Araştırma sürelerini ve bedellerini `research.json` üzerinden güncelleyin.

## 📝 Lisans

Bu proje kişisel gelişim/beta aşamasındadır. Kendi kurallarınıza göre lisans metnini buraya ekleyebilirsiniz (Örn: MIT License).
