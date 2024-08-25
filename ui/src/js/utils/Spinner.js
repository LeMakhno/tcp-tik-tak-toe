const spinnerElement = document.getElementById('spinner')

const spinner = {
    showSpinner() {
        spinnerElement.style.display = 'block'
    },
    hideSpinner() {
        spinnerElement.style.display = 'none'
    }
}

export default spinner
