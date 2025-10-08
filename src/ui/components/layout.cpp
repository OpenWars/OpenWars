#include "components.hpp"
#include "../../core/drawing/shapes.hpp"

void OpenWars::UI::LayoutManager::layoutHorizontal(
    std::vector<Component*>& components,
    const Drawing::Rectangle& bounds,
    const LayoutOptions& options
) {
    if(components.empty())
        return;

    // calc total width needed
    float totalWidth = 0;
    for(const auto& comp : components) {
        totalWidth += comp->getBounds().width;
    }
    totalWidth += options.spacing * (components.size() - 1);

    // start position based on alignment
    float startX = bounds.x + options.padding;
    switch(options.horizontalAlign) {
    case Alignment::Center:
        startX = bounds.x + (bounds.width - totalWidth) / 2.0f;
        break;
    case Alignment::Right:
        startX = bounds.x + bounds.width - totalWidth - options.padding;
        break;
    default:
        break;
    }

    // calc Y position based on vertical alignment
    float y = bounds.y;
    switch(options.verticalAlign) {
    case Alignment::Middle:
        y = bounds.y + bounds.height / 2.0f;
        break;
    case Alignment::Bottom:
        y = bounds.y + bounds.height - options.padding;
        break;
    case Alignment::Top:
        y = bounds.y + options.padding;
        break;
    default:
        break;
    }

    // position components
    float currentX = startX;
    for(auto& comp : components) {
        auto compBounds = comp->getBounds();

        // adjust Y based on component height and alignment
        float compY = y;
        if(options.verticalAlign == Alignment::Middle) {
            compY = y - compBounds.height / 2.0f;
        } else if(options.verticalAlign == Alignment::Top) {
            compY = y + compBounds.height;
        }

        comp->setBounds(currentX, compY, compBounds.width, compBounds.height);
        currentX += compBounds.width + options.spacing;
    }
}

void OpenWars::UI::LayoutManager::layoutVertical(
    std::vector<Component*>& components,
    const Drawing::Rectangle& bounds,
    const LayoutOptions& options
) {
    if(components.empty())
        return;

    // calc total height needed
    float totalHeight = 0;
    for(const auto& comp : components) {
        totalHeight += comp->getBounds().height;
    }
    totalHeight += options.spacing * (components.size() - 1);

    // start position based on alignment
    float startY = bounds.y + options.padding;
    switch(options.verticalAlign) {
    case Alignment::Middle:
        startY = bounds.y + (bounds.height - totalHeight) / 2.0f;
        break;
    case Alignment::Bottom:
        startY = bounds.y + bounds.height - totalHeight - options.padding;
        break;
    default:
        break;
    }

    // calc X position based on horizontal alignment
    float x = bounds.x;

    // position components
    float currentY = startY;
    for(auto& comp : components) {
        auto compBounds = comp->getBounds();

        // calc X based on alignment
        float compX = x + options.padding;
        switch(options.horizontalAlign) {
        case Alignment::Center:
            compX = x + (bounds.width - compBounds.width) / 2.0f;
            break;
        case Alignment::Right:
            compX = x + bounds.width - compBounds.width - options.padding;
            break;
        default:
            break;
        }

        comp->setBounds(
            compX,
            currentY + compBounds.height,
            compBounds.width,
            compBounds.height
        );
        currentY += compBounds.height + options.spacing;
    }
}

void OpenWars::UI::LayoutManager::layoutGrid(
    std::vector<Component*>& components,
    const Drawing::Rectangle& bounds,
    int columns,
    const LayoutOptions& options
) {
    if(components.empty() || columns <= 0)
        return;

    int rows = (components.size() + columns - 1) / columns;

    // calc cell size
    float cellWidth =
        (bounds.width - options.padding * 2 - options.spacing * (columns - 1)) /
        columns;
    float cellHeight =
        (bounds.height - options.padding * 2 - options.spacing * (rows - 1)) /
        rows;

    // position components
    for(size_t i = 0; i < components.size(); ++i) {
        int row = i / columns;
        int col = i % columns;

        float x =
            bounds.x + options.padding + col * (cellWidth + options.spacing);
        float y =
            bounds.y + options.padding + row * (cellHeight + options.spacing);

        auto comp = components[i];
        auto compBounds = comp->getBounds();

        // center component within cell
        float compX = x + (cellWidth - compBounds.width) / 2.0f;
        float compY = y + cellHeight / 2.0f;

        comp->setBounds(compX, compY, compBounds.width, compBounds.height);
    }
}