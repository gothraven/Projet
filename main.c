#include "pdf.h"

int main() {
  pdf_t* p = pdf_create(1, 1300, 520);
  const char* im = pdf_load_image(p, "lenna.jpg");

  char* cnt = NULL;
  asprintf(&cnt,
    "q 1 0 0 -1 0 520 cm "
      "q 2 0 0 2 0 0 cm "
        "q 0 0 1 rg 1 0 0 1 200 130 cm 120 0 0 120 0 0 cm -1 0 m -1 0.553 -0.553 1 0 1 c 0.553 1 1 0.553 1 0 c 1 -0.553 0.553 -1 0 -1 c -0.553 -1 -1 -0.553 -1 0 c F Q "
        "q 0 0.5 0 rg 1 0 0 1 100 80 cm 0.707 0.707 -0.707 0.707 0 0 cm -50 -35 100 70 re F Q "
        "q 10 w 1 0 0 RG 1 0 0 1 180 180 cm 0.5 -0.866 0.866 0.5 0 0 cm -100 0 m 100 0 l S Q "
        "q 1 0 0 1 250 150 cm 0.866 0.5 -0.5 0.866 0 0 cm "
          "q 1 0 1 rg BT /FV 20. Tf 1 0 0 -1 0 0 Tm (Hello, out there) Tj ET Q "
          "q 1 0 0 1 50 -120 cm 100 0 0 -100 0 100 cm /%s Do Q "
        "Q "
      "Q "
    "Q ", im);
  pdf_set_content(p, 0, cnt);
  free(cnt);

  pdf_save("test.pdf",p);
  pdf_delete(p);

  return EXIT_SUCCESS;
}
